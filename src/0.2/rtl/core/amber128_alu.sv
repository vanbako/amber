module amber128_alu
  import amber128_pkg::*;
(
    input  amber128_exec_in_s   ex_i,
    output amber128_exec_out_s  ex_o
);

  amber128_exec_out_s ex_r;
  logic [5:0] shamt;

  always_comb begin
    ex_r               = '0;
    ex_r.valid         = ex_i.valid;
    ex_r.rd            = ex_i.rd;
    ex_r.wb_en         = ex_i.valid;
    ex_r.mem_req       = 1'b0;
    ex_r.mem_we        = 1'b0;
    ex_r.mem_addr      = '0;
    ex_r.mem_wdata     = '0;
    ex_r.branch_taken  = 1'b0;
    ex_r.next_word_addr= ex_i.pc_word_addr + IMEM_WORD_BYTES;
    ex_r.trap          = 1'b0;
    ex_r.trap_cause    = TRAP_NONE;

    shamt = ex_i.op_b[5:0];

    unique case (ex_i.alu_op)
      ALU_PASS: ex_r.result = ex_i.op_b; // mov/pass-through uses op_b as source
      ALU_ADD:  ex_r.result = ex_i.op_a + ex_i.op_b;
      ALU_SUB:  ex_r.result = ex_i.op_a - ex_i.op_b;
      ALU_AND:  ex_r.result = ex_i.op_a & ex_i.op_b;
      ALU_OR:   ex_r.result = ex_i.op_a | ex_i.op_b;
      ALU_XOR:  ex_r.result = ex_i.op_a ^ ex_i.op_b;
      ALU_LSL:  ex_r.result = ex_i.op_a << shamt;
      ALU_LSR:  ex_r.result = ex_i.op_a >> shamt;
      default:  ex_r.result = ex_i.op_a;
    endcase

    // Simple branches on equality/inequality
    unique case (ex_i.branch)
      BR_NONE: begin
        ex_r.branch_taken   = 1'b0;
        ex_r.next_word_addr = ex_i.pc_word_addr + IMEM_WORD_BYTES;
      end
      // For branches, treat imm24 as signed (imm13 sign-extended in decoder)
      BR_UNCOND: begin
        ex_r.branch_taken   = ex_i.valid;
        ex_r.next_word_addr = ex_i.pc_word_addr + ($signed({{40{ex_i.imm24[23]}}, ex_i.imm24}) <<< 4);
      end
      BR_EQ: begin
        ex_r.branch_taken   = (ex_i.op_a == ex_i.op_b);
        ex_r.next_word_addr = ex_i.pc_word_addr + ($signed({{40{ex_i.imm24[23]}}, ex_i.imm24}) <<< 4);
      end
      BR_NE: begin
        ex_r.branch_taken   = (ex_i.op_a != ex_i.op_b);
        ex_r.next_word_addr = ex_i.pc_word_addr + ($signed({{40{ex_i.imm24[23]}}, ex_i.imm24}) <<< 4);
      end
      default: begin
        ex_r.branch_taken   = 1'b0;
        ex_r.next_word_addr = ex_i.pc_word_addr + IMEM_WORD_BYTES;
      end
    endcase

    // 128-bit LD/ST: address comes from cap + (imm24 << 4); default to CREG_DDC
    if (ex_i.is_ld128 || ex_i.is_st128) begin
      ex_r.mem_req  = ex_i.valid;
      ex_r.mem_we   = ex_i.is_st128;
      // Address is provided by core (cap base + scaled imm). ex_r.mem_addr filled in core.
      // ex_r.mem_wdata filled in core as well.
      // No trap logic here; DMEM flags misalign/bounds.
    end
  end

  assign ex_o = ex_r;

endmodule
