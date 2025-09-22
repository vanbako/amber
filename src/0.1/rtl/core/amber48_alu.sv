module amber48_alu
  import amber48_pkg::*;
(
    input  amber48_execute_in_s  ex_i,
    output amber48_execute_out_s ex_o
);

  amber48_execute_out_s ex_r;
  logic [XLEN-1:0]      operand_b;
  logic [5:0]           shamt;
  logic [XLEN-1:0]      branch_cmp_b;
  logic                 trap_active;

  always_comb begin
    ex_r               = '0;
    ex_r.valid         = ex_i.valid;
    ex_r.rd            = ex_i.rd;
    ex_r.store_data    = ex_i.store_data;
    ex_r.writeback_en  = ex_i.writeback_en;
    ex_r.load          = ex_i.load;
    ex_r.store         = ex_i.store;
    ex_r.branch_taken  = 1'b0;
    ex_r.branch_target = ex_i.pc + ex_i.imm;

    operand_b          = ex_i.uses_imm ? ex_i.imm : ex_i.op_b;
    branch_cmp_b       = ex_i.op_b;
    shamt              = operand_b[5:0];

    unique case (ex_i.alu_op)
      ALU_ADD: ex_r.result = ex_i.op_a + operand_b;
      ALU_SUB: ex_r.result = ex_i.op_a - operand_b;
      ALU_AND: ex_r.result = ex_i.op_a & operand_b;
      ALU_OR:  ex_r.result = ex_i.op_a | operand_b;
      ALU_XOR: ex_r.result = ex_i.op_a ^ operand_b;
      ALU_LSL: ex_r.result = ex_i.op_a << shamt;
      ALU_LSR: ex_r.result = ex_i.op_a >> shamt;
      ALU_PASS: ex_r.result = ex_i.uses_imm ? operand_b : ex_i.op_a;
      default: ex_r.result = ex_i.op_a;
    endcase

    unique case (ex_i.branch_type)
      BR_NONE: begin
        ex_r.branch_taken = 1'b0;
      end
      BR_UNCOND: begin
        ex_r.branch_taken = ex_i.valid;
      end
      BR_EQ: begin
        ex_r.branch_taken = (ex_i.op_a == branch_cmp_b);
      end
      BR_NE: begin
        ex_r.branch_taken = (ex_i.op_a != branch_cmp_b);
      end
      BR_LT_U: begin
        ex_r.branch_taken = (ex_i.op_a < branch_cmp_b);
      end
      BR_LT_S: begin
        ex_r.branch_taken = ($signed(ex_i.op_a) < $signed(branch_cmp_b));
      end
      BR_GT_U: begin
        ex_r.branch_taken = (ex_i.op_a > branch_cmp_b);
      end
      BR_GT_S: begin
        ex_r.branch_taken = ($signed(ex_i.op_a) > $signed(branch_cmp_b));
      end
      BR_ZERO: begin
        ex_r.branch_taken = (ex_i.op_a == '0);
      end
      BR_NOT_ZERO: begin
        ex_r.branch_taken = (ex_i.op_a != '0);
      end
      default: begin
        ex_r.branch_taken = 1'b0;
      end
    endcase

    trap_active       = ex_i.trap || (ex_i.trap_cause != TRAP_NONE);
    ex_r.trap         = trap_active;
    ex_r.trap_cause   = trap_active ? (ex_i.trap_cause != TRAP_NONE ? ex_i.trap_cause : TRAP_ILLEGAL)
                                    : TRAP_NONE;
  end

  assign ex_o = ex_r;

endmodule
