`timescale 1ns/1ps

module amber128_core
  import amber128_pkg::*;
(
    input  logic                   clk_i,
    input  logic                   rst_ni,
    input  logic                   clk_en_i,
    // IMEM: 128-bit bundle fetch
    output logic [63:0]            imem_addr_o,
    input  logic [C_XLEN-1:0]      imem_data_i,
    input  logic                   imem_valid_i,
    // Trap & retire
    output logic                   trap_o,
    output logic [2:0]             trap_cause_o,
    output logic                   retired_o,
    // DMEM: 128-bit load/store
    output logic                   dmem_req_o,
    output logic                   dmem_we_o,
    output logic [63:0]            dmem_addr_o,
    output logic [C_XLEN-1:0]      dmem_wdata_o,
    input  logic [C_XLEN-1:0]      dmem_rdata_i,
    input  logic                   dmem_ready_i,
    input  logic                   dmem_trap_i
);

  // Program counter state (bundle address + 5-slot position)
  logic [63:0]           pc_word_addr_q, pc_word_addr_n;
  logic [2:0]            slot_idx_q, slot_idx_n;   // 0..4
  logic                  sub12_q, sub12_n;         // 0 or 1
  logic                  bundle_valid_q, bundle_valid_n;
  logic [C_XLEN-1:0]     bundle_q, bundle_n;

  // Fetch/decode/execute wires
  amber128_fetch_s       fetch_bus;
  /* verilator lint_off UNUSEDSIGNAL */
  amber128_decode_s      decode_bus;
  amber128_exec_out_s    ex_o;
  /* verilator lint_on UNUSEDSIGNAL */
  logic                  slot_two12;
  logic [D_XLEN-1:0]     imm64;

  amber128_regfile_req_s rf_req;
  logic [D_XLEN-1:0]     rf_rs;
  logic [D_XLEN-1:0]     rf_rd_curr;
  logic                   rf_we_commit;
  logic [D_XLEN-1:0]      rf_wd_commit;
  logic [DATA_REG_AW-1:0] rf_wr_addr_commit;

  amber128_exec_in_s     ex_i;

  // Capability access (PC, addressing cap, data cap)
  logic [C_XLEN-1:0]     cap_pc_rdata;
  logic [C_XLEN-1:0]     cap_addr_rdata;
  logic [C_XLEN-1:0]     cap_data_rdata;
  logic                   cap_we;
  logic [CAP_REG_AW-1:0]  cap_waddr;
  logic [C_XLEN-1:0]      cap_wdata;

  // Control
  logic                   pipeline_stall;
  logic                   mem_inflight_q, mem_inflight_n;
  logic [63:0]            mem_addr_q, mem_addr_n;
  logic [C_XLEN-1:0]      mem_wdata_q, mem_wdata_n;
  logic                   mem_we_q, mem_we_n;
  logic                   trap_q, trap_n;
  amber128_trap_e         trap_cause_q, trap_cause_n;
  logic                   retired_pulse;

  // Submodules
  amber128_regfile u_regfile (
      .clk_i    (clk_i),
      .rst_ni   (rst_ni),
      .req_i    (rf_req),
      .wr_en_i  (rf_we_commit),
      .wr_addr_i(rf_wr_addr_commit),
      .wr_data_i(rf_wd_commit),
      .rd_a_o   (rf_rs),
      .rd_b_o   (rf_rd_curr)
  );

  amber128_capfile u_capfile (
      .clk_i    (clk_i),
      .rst_ni   (rst_ni),
      .we_i     (cap_we),
      .waddr_i  (cap_waddr),
      .wdata_i  (cap_wdata),
      .raddr0_i (CREG_PC),
      .rdata0_o (cap_pc_rdata),
      .raddr1_i (decode_bus.cap_addr_sel),
      .rdata1_o (cap_addr_rdata),
      .raddr2_i (decode_bus.cap_data_sel),
      .rdata2_o (cap_data_rdata)
  );

  amber128_alu u_alu (
      .ex_i (ex_i),
      .ex_o (ex_o)
  );

  // Fetch bus assembly
  always_comb begin
    fetch_bus.valid     = bundle_valid_q;
    fetch_bus.word_addr = pc_word_addr_q;
    fetch_bus.bundle    = bundle_q;
  end

  amber128_decoder u_decoder (
      .fetch_i           (fetch_bus),
      .slot_idx_i        (slot_idx_q),
      .sub12_i           (sub12_q),
      .decode_o          (decode_bus),
      .slot_has_two12_o  (slot_two12)
  );

  // Instruction memory interface always points to current word address
  assign imem_addr_o = pc_word_addr_q;

  // Regfile request: rs read and rd current value read (for passthrough)
  always_comb begin
    rf_req          = '0;
    if (decode_bus.cap_move) begin
      rf_req.ra     = decode_bus.cap_move_src_hi;
      rf_req.rb     = decode_bus.cap_move_src_lo;
    end else if (decode_bus.is_st128 || decode_bus.is_ld128) begin
      rf_req.ra     = REG_ZERO;
      rf_req.rb     = REG_ZERO;
    end else begin
      rf_req.ra     = decode_bus.rd; // rd current value
      rf_req.rb     = decode_bus.is_imm ? REG_ZERO : decode_bus.rs; // rs value or literal
    end
  end

  // Build execute input
  always_comb begin
    imm64               = {{(D_XLEN-24){decode_bus.imm24[23]}}, decode_bus.imm24};
    ex_i                = '0;
    ex_i.valid          = decode_bus.valid && !pipeline_stall && !trap_q;
    ex_i.pc_word_addr   = decode_bus.pc_word_addr;
    ex_i.rd             = decode_bus.rd;
    ex_i.alu_op         = decode_bus.alu_op;
    ex_i.is_imm         = decode_bus.is_imm;
    ex_i.imm24          = decode_bus.imm24;
    ex_i.branch         = decode_bus.branch;
    ex_i.is_ld128       = decode_bus.is_ld128;
    ex_i.is_st128       = decode_bus.is_st128;
    ex_i.op_a           = rf_rd_curr;
    ex_i.op_b           = decode_bus.is_imm ? imm64 : rf_rs;
  end

  // Compute memory micro-ops and address using addressing capability base + (imm << 4)
  logic [63:0] cap_base;
  logic [63:0] cap_bound;
  logic [63:0] eff_addr;
  logic        cap_ok_mem;
  logic        cap_ok_pc;
  logic [63:0] pc_base;
  logic [63:0] pc_bound;
  always_comb begin
    cap_base  = cap_addr_rdata[63:0];
    cap_bound = cap_addr_rdata[127:64];
    eff_addr  = cap_base + ( {40'b0, decode_bus.imm24} << 4 );
    cap_ok_mem= (eff_addr >= cap_base) && ((eff_addr + 16) <= cap_bound);
    // PC capability bounds check for current bundle address
    pc_base   = cap_pc_rdata[63:0];
    pc_bound  = cap_pc_rdata[127:64];
    cap_ok_pc = (pc_word_addr_q >= pc_base) && ((pc_word_addr_q + 16) <= pc_bound);
  end

  // Memory request tracking
  assign dmem_req_o   = mem_inflight_q;
  assign dmem_we_o    = mem_we_q;
  assign dmem_addr_o  = mem_addr_q;
  assign dmem_wdata_o = mem_wdata_q;

  // Pipeline control and state updates
  logic init_caps_q, init_caps_n;

  always_comb begin
    // Defaults
    bundle_valid_n    = bundle_valid_q;
    bundle_n          = bundle_q;
    slot_idx_n        = slot_idx_q;
    sub12_n           = sub12_q;
    pc_word_addr_n    = pc_word_addr_q;
    mem_inflight_n    = mem_inflight_q;
    mem_addr_n        = mem_addr_q;
    mem_wdata_n       = mem_wdata_q;
    mem_we_n          = mem_we_q;
    trap_n            = trap_q;
    trap_cause_n      = trap_cause_q;
    retired_pulse     = 1'b0;
    cap_we            = 1'b0;
    cap_waddr         = '0;
    cap_wdata         = '0;
    init_caps_n       = init_caps_q;
    rf_we_commit      = 1'b0;
    rf_wd_commit      = '0;
    rf_wr_addr_commit = '0;

    // Initialize PC CAR once after reset
    if (init_caps_q) begin
      cap_we      = 1'b1;
      cap_waddr   = CREG_PC;
      cap_wdata   = {64'hFFFF_FFFF_FFFF_FFFF, 64'h0};
      init_caps_n = 1'b0;
    end

    // Fetch next bundle when needed
    if (!bundle_valid_q && imem_valid_i) begin
      bundle_n       = imem_data_i;
      bundle_valid_n = 1'b1;
      slot_idx_n     = 3'd0;
      sub12_n        = 1'b0;
    end

    // Advance within bundle after execute if no stall
    if (decode_bus.valid && !pipeline_stall && !trap_q) begin
      // Branch control
      if (ex_o.branch_taken) begin
        // Redirect fetch to branch target bundle address
        bundle_valid_n = 1'b0;
        pc_word_addr_n = ex_o.next_word_addr;
        slot_idx_n     = 3'd0;
        sub12_n        = 1'b0;
        // Update PC capability base to new bundle address (bound unchanged)
        cap_we    = 1'b1;
        cap_waddr = CREG_PC;
        cap_wdata = {cap_pc_rdata[127:64], pc_word_addr_n};
        retired_pulse = 1'b1;
      end else begin
      // Memory operations and capability transfers
      if (decode_bus.cap_move) begin
        cap_we    = 1'b1;
        cap_waddr = decode_bus.cap_move_dst;
        cap_wdata = {rf_rs, rf_rd_curr};
        retired_pulse = 1'b1;
      end else if (decode_bus.is_st128) begin
        // Source pair: rd and rd+1 -> 128b
        if (!cap_ok_mem) begin
          trap_n       = 1'b1;
          trap_cause_n = TRAP_DATA_FAULT;
        end else begin
          mem_addr_n     = eff_addr;
          mem_wdata_n    = cap_data_rdata;
          mem_we_n       = 1'b1;
          mem_inflight_n = 1'b1;
        end
      end else if (decode_bus.is_ld128) begin
        if (!cap_ok_mem) begin
          trap_n       = 1'b1;
          trap_cause_n = TRAP_DATA_FAULT;
        end else begin
          mem_addr_n     = eff_addr;
          mem_wdata_n    = '0;
          mem_we_n       = 1'b0;
          mem_inflight_n = 1'b1;
        end
      end else begin
        // Non-mem: retire and prepare writeback
        if (decode_bus.rd != REG_ZERO) begin
          rf_we_commit      = 1'b1;
          rf_wd_commit      = ex_o.result;
          rf_wr_addr_commit = decode_bus.rd;
        end
        retired_pulse = 1'b1;
      end

      // Advance slot/sub-instruction position
      if (slot_two12 && (sub12_q == 1'b0) && !(decode_bus.is_ld128 || decode_bus.is_st128)) begin
        // Two 12-bit ops: move to second in same slot (memory ops are only 24-bit currently)
        sub12_n = 1'b1;
      end else begin
        sub12_n = 1'b0;
        if (slot_idx_q == 3'd4) begin
          bundle_valid_n = 1'b0;
          pc_word_addr_n = pc_word_addr_q + 64'(IMEM_WORD_BYTES);
          // Update PC capability base to new bundle address (bound unchanged)
          if (!(mem_inflight_q && dmem_ready_i)) begin
            if (!cap_we) begin
              cap_we    = 1'b1;
              cap_waddr = CREG_PC;
              cap_wdata = {cap_pc_rdata[127:64], pc_word_addr_n};
            end
          end
        end else begin
          slot_idx_n = slot_idx_q + 3'd1;
        end
      end
      end // not a branch
    end

    // Handle memory completion
    if (mem_inflight_q && dmem_ready_i) begin
      if (dmem_trap_i) begin
        trap_n       = 1'b1;
        trap_cause_n = TRAP_DATA_FAULT;
      end else begin
        if (!mem_we_q) begin
          // LD128: write back into capability cap_data_sel
          cap_we    = 1'b1;
          cap_waddr = decode_bus.cap_data_sel;
          cap_wdata = dmem_rdata_i;
        end
        retired_pulse = 1'b1;
      end
      mem_inflight_n = 1'b0;
    end
  end

  assign pipeline_stall = mem_inflight_q && !dmem_ready_i;

  // Sequential state
  always_ff @(posedge clk_i or negedge rst_ni) begin
    if (!rst_ni) begin
      pc_word_addr_q <= 64'd0;
      slot_idx_q     <= 3'd0;
      sub12_q        <= 1'b0;
      bundle_valid_q <= 1'b0;
      bundle_q       <= '0;
      mem_inflight_q <= 1'b0;
      mem_addr_q     <= '0;
      mem_wdata_q    <= '0;
      mem_we_q       <= 1'b0;
      trap_q         <= 1'b0;
      trap_cause_q   <= TRAP_NONE;
      init_caps_q    <= 1'b1;
    end else if (clk_en_i) begin
      pc_word_addr_q <= pc_word_addr_n;
      slot_idx_q     <= slot_idx_n;
      sub12_q        <= sub12_n;
      bundle_valid_q <= bundle_valid_n;
      bundle_q       <= bundle_n;
      mem_inflight_q <= mem_inflight_n;
      mem_addr_q     <= mem_addr_n;
      mem_wdata_q    <= mem_wdata_n;
      mem_we_q       <= mem_we_n;
      trap_q         <= trap_n;
      trap_cause_q   <= trap_cause_n;
      init_caps_q    <= init_caps_n;
    end
  end

  assign trap_o        = trap_q || (!cap_ok_pc && !init_caps_q);
  assign trap_cause_o  = trap_cause_q;
  assign retired_o     = retired_pulse;

  // Initialize and maintain CARs: on reset, set PC bound to max and base 0; keep SP/SSP/LR zero.
  // Folded into main control to avoid multiple drivers.

endmodule
