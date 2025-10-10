`timescale 1ns/1ps
`ifdef CPU_AD48_INSTR_VH
`undef CPU_AD48_INSTR_VH
`endif

module cpu_ad48_nested_tb;
  localparam IM_WORDS  = 128;
  localparam DM_WORDS  = 32;
  localparam [47:0] TRAP_VEC = 48'd64;
  localparam integer TRAP_IDX = 64;
  localparam IRQ_LINES = 4;

  reg clk;
  reg resetn;
  wire [IRQ_LINES-1:0] irq_lines = {IRQ_LINES{1'b0}};

  cpu_ad48 #(
    .IM_WORDS(IM_WORDS),
    .DM_WORDS(DM_WORDS),
    .TRAP_VECTOR(TRAP_VEC),
    .IRQ_LINES(IRQ_LINES)
  ) dut (
    .clk   (clk),
    .resetn(resetn),
    .irq   (irq_lines)
  );

  `include "../../src/rtl/cpu_ad48_instr.vh"

  localparam [47:0] BASE_SSP      = to48(48'd32);
  localparam [47:0] MID_SSP       = BASE_SSP + to48(48'd1);
  localparam [47:0] FINAL_SSP     = BASE_SSP + to48(48'd2);
  localparam [47:0] HALT_PC       = to48(48'd1);
  localparam [47:0] SCRATCH_DONE  = to48(48'hAA);
  localparam integer MAX_CYCLES   = 400;

  integer pc;
  integer branch_depth_idx;
  integer depth1_idx;
  integer nested_sys_idx;
  integer depth0_after_idx;

  initial begin
    clk = 1'b0;
    forever #5 clk = ~clk;
  end

  task init_mem;
    integer idx;
    begin
      for (idx = 0; idx < IM_WORDS; idx = idx + 1) begin
        dut.IMEM.mem[idx] = 48'd0;
      end
      for (idx = 0; idx < DM_WORDS; idx = idx + 1) begin
        dut.DMEM.mem[idx] = 48'd0;
      end
    end
  endtask

  task load_program;
    integer depth1_offset;
    begin
      init_mem();
      pc = 0;
      // Main program: trigger breakpoint, then halt after handlers complete.
      dut.IMEM.mem[pc++] = instr_sys(4'h1); // breakpoint -> first trap
      dut.IMEM.mem[pc++] = instr_sys(4'hF); // resume target after nested traps

      pc = TRAP_IDX;
      // Handler prologue: depth detection
      dut.IMEM.mem[pc++] = instr_csr   (CSR_F_R,  1'b1, 3'd0, 3'd0, pack_csr_addr(CSR_ADDR_SCRATCH)); // D0 = scratch (depth)
      branch_depth_idx   = pc;
      dut.IMEM.mem[pc++] = instr_br    (C_BNE, 3'd0, 3'd0, pack_off31(to48(48'd0))); // patched later

      // Depth 0 handler body
      dut.IMEM.mem[pc++] = instr_alui_d(1'b1, 3'd1, 3'd1, pack_subop(F_AND), pack_imm27(to48(48'd0))); // D1 = 0
      dut.IMEM.mem[pc++] = instr_alui_d(1'b1, 3'd1, 3'd1, pack_subop(F_ADD), pack_imm27(to48(48'd1))); // D1 = 1
      dut.IMEM.mem[pc++] = instr_csr   (CSR_F_RW, 1'b0, 3'd0, 3'd1, pack_csr_addr(CSR_ADDR_SCRATCH));  // scratch = 1
      dut.IMEM.mem[pc++] = instr_alui_d(1'b1, 3'd2, 3'd2, pack_subop(F_AND), pack_imm27(to48(48'd0))); // D2 = 0
      dut.IMEM.mem[pc++] = instr_alui_d(1'b1, 3'd2, 3'd2, pack_subop(F_ADD), pack_imm27(BASE_SSP));    // D2 = BASE_SSP
      dut.IMEM.mem[pc++] = instr_csr   (CSR_F_RW, 1'b0, 3'd0, 3'd2, pack_csr_addr(CSR_ADDR_SSP));      // SSP = BASE_SSP
      dut.IMEM.mem[pc++] = instr_alui_a(1'b0, 3'd7, 3'd7, pack_subop(F_ADD), pack_imm27(to48(48'd1))); // A7 += 1 -> SSP = BASE+1
      nested_sys_idx     = pc;
      dut.IMEM.mem[pc++] = instr_sys   (4'h1);                                                         // nested breakpoint
      depth0_after_idx   = pc;
      dut.IMEM.mem[pc++] = instr_alui_d(1'b1, 3'd3, 3'd3, pack_subop(F_AND), pack_imm27(to48(48'd0))); // D3 = 0
      dut.IMEM.mem[pc++] = instr_alui_d(1'b1, 3'd3, 3'd3, pack_subop(F_ADD), pack_imm27(SCRATCH_DONE)); // D3 = 0xAA
      dut.IMEM.mem[pc++] = instr_csr   (CSR_F_RW, 1'b0, 3'd0, 3'd3, pack_csr_addr(CSR_ADDR_SCRATCH));   // scratch = 0xAA
      dut.IMEM.mem[pc++] = instr_alui_d(1'b1, 3'd4, 3'd4, pack_subop(F_AND), pack_imm27(to48(48'd0))); // D4 = 0
      dut.IMEM.mem[pc++] = instr_alui_d(1'b1, 3'd4, 3'd4, pack_subop(F_ADD), pack_imm27(HALT_PC));     // D4 = 1
      dut.IMEM.mem[pc++] = instr_csr   (CSR_F_RW, 1'b0, 3'd0, 3'd4, pack_csr_addr(CSR_ADDR_LR));        // LR <- 1
      dut.IMEM.mem[pc++] = instr_sys   (4'h2);                                                          // IRET outer

      // Depth 1 handler body
      depth1_idx         = pc;
      dut.IMEM.mem[pc++] = instr_csr   (CSR_F_R,  1'b1, 3'd1, 3'd0, pack_csr_addr(CSR_ADDR_LR));       // D1 = LR (nested EPC)
      dut.IMEM.mem[pc++] = instr_csr   (CSR_F_R,  1'b1, 3'd2, 3'd0, pack_csr_addr(CSR_ADDR_SSP));      // D2 = SSP (BASE+1)
      dut.IMEM.mem[pc++] = instr_alui_d(1'b1, 3'd3, 3'd3, pack_subop(F_AND), pack_imm27(to48(48'd0))); // D3 = 0
      dut.IMEM.mem[pc++] = instr_alui_d(1'b1, 3'd3, 3'd3, pack_subop(F_ADD), pack_imm27(to48(48'd2))); // D3 = 2
      dut.IMEM.mem[pc++] = instr_csr   (CSR_F_RW, 1'b0, 3'd0, 3'd3, pack_csr_addr(CSR_ADDR_SCRATCH));   // scratch = 2
      dut.IMEM.mem[pc++] = instr_alui_a(1'b0, 3'd7, 3'd7, pack_subop(F_ADD), pack_imm27(to48(48'd1))); // A7 += 1 -> SSP = BASE+2
      dut.IMEM.mem[pc++] = instr_csr   (CSR_F_R,  1'b1, 3'd5, 3'd0, pack_csr_addr(CSR_ADDR_SSP));      // D5 = SSP after push
      dut.IMEM.mem[pc++] = instr_alui_d(1'b1, 3'd4, 3'd4, pack_subop(F_AND), pack_imm27(to48(48'd0))); // D4 = 0
      dut.IMEM.mem[pc++] = instr_alui_d(1'b1, 3'd4, 3'd4, pack_subop(F_ADD), pack_imm27(to48(depth0_after_idx))); // D4 = depth0 resume
      dut.IMEM.mem[pc++] = instr_csr   (CSR_F_RW, 1'b0, 3'd0, 3'd4, pack_csr_addr(CSR_ADDR_LR));        // LR <- depth0_after
      dut.IMEM.mem[pc++] = instr_sys   (4'h2);                                                          // IRET inner

      depth1_offset = depth1_idx - (branch_depth_idx + 1);
      dut.IMEM.mem[branch_depth_idx] = instr_br(
        C_BNE,
        3'd0,
        3'd0,
        pack_off31(to48(depth1_offset))
      );
    end
  endtask

  task wait_for_halt;
    integer cycles;
    begin
      cycles = 0;
      while (!dut.halt && (cycles < MAX_CYCLES)) begin
        @(posedge clk);
        cycles = cycles + 1;
      end
      if (!dut.halt) begin
        $display("NESTED TB: timeout waiting for HALT (cycles=%0d)", cycles);
        $fatal(1);
      end
      @(posedge clk);
    end
  endtask

  task check_final_state;
    begin
      if (dut.pc !== HALT_PC) begin
        $display("NESTED TB: final PC mismatch. Got %h expected %h", dut.pc, HALT_PC);
        $fatal(1);
      end
      if (dut.csr_lr !== HALT_PC) begin
        $display("NESTED TB: LR mismatch. Got %h expected %h", dut.csr_lr, HALT_PC);
        $fatal(1);
      end
      if (dut.csr_epc !== HALT_PC) begin
        $display("NESTED TB: EPC mismatch. Got %h expected %h", dut.csr_epc, HALT_PC);
        $fatal(1);
      end
      if (dut.csr_ssp !== FINAL_SSP) begin
        $display("NESTED TB: SSP mismatch. Got %h expected %h", dut.csr_ssp, FINAL_SSP);
        $fatal(1);
      end
      if (dut.RF_A.regs[7] !== 48'd0) begin
        $display("NESTED TB: A7 user context modified. value=%h", dut.RF_A.regs[7]);
        $fatal(1);
      end
      if (dut.RF_D.regs[1] !== to48(nested_sys_idx)) begin
        $display("NESTED TB: D1 (captured LR) mismatch. Got %h expected %h", dut.RF_D.regs[1], to48(nested_sys_idx));
        $fatal(1);
      end
      if (dut.RF_D.regs[2] !== MID_SSP) begin
        $display("NESTED TB: D2 (mid SSP) mismatch. Got %h expected %h", dut.RF_D.regs[2], MID_SSP);
        $fatal(1);
      end
      if (dut.RF_D.regs[5] !== FINAL_SSP) begin
        $display("NESTED TB: D5 (final SSP) mismatch. Got %h expected %h", dut.RF_D.regs[5], FINAL_SSP);
        $fatal(1);
      end
      if (dut.csr_scratch !== SCRATCH_DONE) begin
        $display("NESTED TB: scratch mismatch. Got %h expected %h", dut.csr_scratch, SCRATCH_DONE);
        $fatal(1);
      end
      if (dut.handler_active !== 1'b0) begin
        $display("NESTED TB: handler_active stuck high after traps");
        $fatal(1);
      end
    end
  endtask

  initial begin
    resetn = 1'b0;
    load_program();
    repeat (4) @(posedge clk);
    resetn = 1'b1;
    wait_for_halt();
    check_final_state();
    $display("cpu_ad48_nested_tb PASS");
    $finish;
  end
endmodule
