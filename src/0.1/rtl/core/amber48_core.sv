module amber48_core
  import amber48_pkg::*;
(
    input  logic                 clk_i,
    input  logic                 rst_ni,
    input  logic                 clk_en_i,
    output logic [XLEN-1:0]      imem_addr_o,
    input  logic [XLEN-1:0]      imem_data_i,
    input  logic                 imem_valid_i,
    output logic                 trap_o,
    output logic [2:0]      trap_cause_o,
    output logic                 retired_o,
    // Data memory interface
    output logic                 dmem_req_o,
    output logic                 dmem_we_o,
    output logic [XLEN-1:0]      dmem_addr_o,
    output logic [XLEN-1:0]      dmem_wdata_o,
    input  logic [XLEN-1:0]      dmem_rdata_i,
    input  logic                 dmem_ready_i,
    input  logic                 dmem_trap_i
);

    localparam logic [XLEN-1:0]           PC_INCREMENT = XLEN'(BAU_BYTES);

  logic [XLEN-1:0]        pc_q;
  logic [XLEN-1:0]        pc_next;

  amber48_decode_in_s     if_stage_q;
  amber48_decode_in_s     if_stage_next;
  amber48_decode_out_s    id_stage_q;
  amber48_decode_out_s    id_stage_next;
  amber48_decode_out_s    id_stage_from_decoder;
  amber48_execute_in_s    ex_stage_q;
  amber48_execute_in_s    ex_stage_next;
  amber48_execute_out_s   ex_stage_result_raw;
  amber48_execute_out_s   ex_stage_result;

  amber48_regfile_req_s   rf_req;
  logic [XLEN-1:0]        rf_rs1;
  logic [XLEN-1:0]        rf_rs2;

  logic [XLEN-1:0]        writeback_data;
  logic                   writeback_en;
  logic                   store_commit;
  logic                   mem_request_pending;
  logic                   pipeline_stall;

  amber48_decoder u_decoder (
      .fetch_i (if_stage_q),
      .decode_o(id_stage_from_decoder)
  );

  amber48_regfile u_regfile (
      .clk_i (clk_i),
      .rst_ni(rst_ni),
      .req_i (rf_req),
      .rd_a_o(rf_rs1),
      .rd_b_o(rf_rs2)
  );

  amber48_alu u_alu (
      .ex_i(ex_stage_q),
      .ex_o(ex_stage_result_raw)
  );

  always_comb begin
    ex_stage_result = ex_stage_result_raw;
    if (ex_stage_result_raw.valid && (ex_stage_result_raw.load || ex_stage_result_raw.store) &&
        dmem_ready_i && dmem_trap_i) begin
      ex_stage_result.trap       = 1'b1;
      ex_stage_result.trap_cause = TRAP_DATA_FAULT;
    end
  end

  assign mem_request_pending = ex_stage_q.valid && !ex_stage_q.trap &&
                               (ex_stage_q.load || ex_stage_q.store);
  assign pipeline_stall      = mem_request_pending && !dmem_ready_i;

  // Imem request always points at the current PC.
  assign imem_addr_o = pc_q;

  // Data memory requests originate from the execute stage result.
  assign dmem_req_o   = ex_stage_result.valid && !ex_stage_result.trap &&
                        (ex_stage_result.load || ex_stage_result.store);
  assign dmem_we_o    = ex_stage_result.store;
  assign dmem_addr_o  = ex_stage_result.result;
  assign dmem_wdata_o = ex_stage_result.store_data;

  assign writeback_data = ex_stage_result.load ? dmem_rdata_i : ex_stage_result.result;
  assign writeback_en   = ex_stage_result.valid && ex_stage_result.writeback_en &&
                          !ex_stage_result.trap &&
                          (!ex_stage_result.load || dmem_ready_i);
  assign store_commit   = ex_stage_result.valid && ex_stage_result.store &&
                          !ex_stage_result.trap && dmem_ready_i;

  always_comb begin
    rf_req            = '0;
    rf_req.valid      = 1'b1;
    rf_req.addr_a     = id_stage_q.rs1;
    rf_req.addr_b     = id_stage_q.rs2;
    rf_req.addr_w     = ex_stage_result.rd;
    rf_req.we         = writeback_en;
    rf_req.wd         = writeback_data;
  end

  // Pipeline next-state logic
  always_comb begin
    pc_next       = pc_q + PC_INCREMENT;
    if_stage_next = if_stage_q;
    id_stage_next = id_stage_from_decoder;
    ex_stage_next = ex_stage_q;

    if (imem_valid_i) begin
      if_stage_next.valid = 1'b1;
      if_stage_next.pc    = pc_q;
      if_stage_next.instr = imem_data_i;
    end else if (!pipeline_stall) begin
      if_stage_next       = '0;
    end

    ex_stage_next              = '0;
    ex_stage_next.valid        = id_stage_q.valid;
    ex_stage_next.pc           = id_stage_q.pc;
    ex_stage_next.op_a         = rf_rs1;
    ex_stage_next.op_b         = rf_rs2;
    ex_stage_next.imm          = id_stage_q.imm;
    ex_stage_next.uses_imm     = id_stage_q.uses_imm;
    ex_stage_next.alu_op       = id_stage_q.alu_op;
    ex_stage_next.branch_type  = id_stage_q.branch_type;
    ex_stage_next.is_jump      = id_stage_q.is_jump;
    ex_stage_next.is_jump_sub  = id_stage_q.is_jump_sub;
    ex_stage_next.is_return    = id_stage_q.is_return;
    ex_stage_next.rd           = id_stage_q.rd;
    ex_stage_next.store_data   = rf_rs2;
    ex_stage_next.writeback_en = id_stage_q.valid && !id_stage_q.trap &&
                                 ((!id_stage_q.store &&
                                   (id_stage_q.branch_type == BR_NONE) &&
                                   (id_stage_q.rd != REG_ZERO)) ||
                                  id_stage_q.is_jump_sub);
    ex_stage_next.load         = id_stage_q.load;
    ex_stage_next.store        = id_stage_q.store;
    ex_stage_next.trap         = id_stage_q.trap;
    ex_stage_next.trap_cause   = id_stage_q.trap_cause;

    if (id_stage_q.uses_imm) begin
      ex_stage_next.op_b = id_stage_q.imm;
    end

    if (ex_stage_result.valid && ex_stage_result.branch_taken) begin
      pc_next       = ex_stage_result.branch_target;
      if_stage_next = '0;
      id_stage_next = '0;
    end

    if (ex_stage_result.valid && ex_stage_result.trap) begin
      if_stage_next = '0;
      id_stage_next = '0;
    end

    if (pipeline_stall) begin
      pc_next       = pc_q;
      if_stage_next = if_stage_q;
      id_stage_next = id_stage_q;
      ex_stage_next = ex_stage_q;
    end
  end

  // Sequential pipeline state
  always_ff @(posedge clk_i or negedge rst_ni) begin
    if (!rst_ni) begin
      pc_q        <= '0;
      if_stage_q  <= '0;
      id_stage_q  <= '0;
      ex_stage_q  <= '0;
    end else if (clk_en_i) begin
      pc_q        <= pc_next;
      if_stage_q  <= if_stage_next;
      id_stage_q  <= id_stage_next;
      ex_stage_q  <= ex_stage_next;
    end
  end

  assign trap_o       = ex_stage_result.valid && ex_stage_result.trap;
  assign trap_cause_o = ex_stage_result.trap_cause;
  assign retired_o    = writeback_en || store_commit;

endmodule
