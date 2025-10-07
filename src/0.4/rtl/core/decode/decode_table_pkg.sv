// Amber v0.4 decode helper package.
// Supplies tasks to map ISA encodings into micro-op tags.

package decode_table_pkg;

  import uop_pkg::*;

  task automatic decode_short_instr(
    input  logic [11:0] instr,
    output logic        matched,
    output uop_tag_t    tag0,
    output uop_tag_t    tag1,
    output logic [1:0]  count
  );
    matched = 1'b0;
    tag0    = UOP_INT_ALU;
    tag1    = UOP_INT_ALU;
    count   = 2'd0;

    if ((instr & 12'hE07) == 12'hE00) begin
      matched = 1'b1;
      tag0    = UOP_PREFIX_SELECT;
      count   = 2'd1;
    end else if (instr == 12'hE07) begin
      matched = 1'b1;
      tag0    = UOP_PREFIX_CANCEL;
      count   = 2'd1;
    end else if ((instr & 12'hE07) == 12'hA00) begin
      matched = 1'b1;
      tag0    = UOP_INT_SAT_ADD;
      count   = 2'd1;
    end else if ((instr & 12'hE07) == 12'hA01) begin
      matched = 1'b1;
      tag0    = UOP_INT_SAT_SUB;
      count   = 2'd1;
    end else if ((instr & 12'hE03) == 12'h800) begin
      matched = 1'b1;
      tag0    = UOP_LD_U8;
      count   = 2'd1;
    end else if ((instr & 12'hE03) == 12'h801) begin
      matched = 1'b1;
      tag0    = UOP_ST_U8;
      count   = 2'd1;
    end else if ((instr & 12'hE03) == 12'hC02) begin
      matched = 1'b1;
      tag0    = UOP_CSR_ALIAS_RD;
      count   = 2'd1;
    end
  endtask

  task automatic decode_long_instr(
    input  logic [23:0] instr,
    output logic        matched,
    output uop_tag_t    tag0,
    output uop_tag_t    tag1,
    output logic [1:0]  count
  );
    matched = 1'b0;
    tag0    = UOP_INT_ALU;
    tag1    = UOP_INT_ALU;
    count   = 2'd0;

    if ((instr & 24'hF000FF) == 24'h500000) begin
      matched = 1'b1;
      tag0    = UOP_PACK_ADD_SAT;
      count   = 2'd1;
    end else if ((instr & 24'hF000FF) == 24'h500010) begin
      matched = 1'b1;
      tag0    = UOP_PACK_SUB_SAT;
      count   = 2'd1;
    end else if ((instr & 24'hF000FF) == 24'h500020) begin
      matched = 1'b1;
      tag0    = UOP_PACK_AVG;
      count   = 2'd1;
    end else if ((instr & 24'hF000FF) == 24'h500030) begin
      matched = 1'b1;
      tag0    = UOP_PACK_MINMAX;
      count   = 2'd1;
    end else if ((instr & 24'hF0000F) == 24'h600000) begin
      matched = 1'b1;
      tag0    = UOP_MEM_PREFETCH;
      count   = 2'd1;
    end else if ((instr & 24'hFFFFF0) == 24'h700000) begin
      matched = 1'b1;
      tag0    = UOP_CAP_FENCE_PREFIX;
      count   = 2'd1;
    end else if ((instr & 24'hF00000) == 24'h800000) begin
      matched = 1'b1;
      tag0    = UOP_CAP_CLONE_RESTRICT;
      count   = 2'd1;
    end else if ((instr & 24'hF00FFF) == 24'h900FF1) begin
      matched = 1'b1;
      tag0    = UOP_CAP_LOAN_END;
      count   = 2'd1;
    end else if ((instr & 24'hF00000) == 24'h900000) begin
      matched = 1'b1;
      tag0    = UOP_CAP_LOAN_BEGIN;
      count   = 2'd1;
    end else if ((instr & 24'hF00001) == 24'h300000) begin
      matched = 1'b1;
      tag0    = UOP_INT_BRANCH;
      count   = 2'd1;
    end else if ((instr & 24'hFF0003) == 24'h400000) begin
      matched = 1'b1;
      tag0    = UOP_CAP_JUMP;
      tag1    = UOP_LINK;
      count   = 2'd2;
    end else if ((instr & 24'hF00003) == 24'h200000) begin
      matched = 1'b1;
      tag0    = UOP_CAP_JUMP;
      count   = 2'd1;
    end else if ((instr & 24'hF0FFF3) == 24'h200001) begin
      matched = 1'b1;
      tag0    = UOP_CAP_RET;
      count   = 2'd1;
    end else if ((instr & 24'hF00FFF) == 24'hC00000) begin
      matched = 1'b1;
      tag0    = UOP_LR128;
      count   = 2'd1;
    end else if ((instr & 24'hF00FFF) == 24'hC00001) begin
      matched = 1'b1;
      tag0    = UOP_SC128;
      count   = 2'd1;
    end else if ((instr & 24'hF00FFF) == 24'hC00002) begin
      matched = 1'b1;
      tag0    = UOP_CAS128;
      count   = 2'd1;
    end else if ((instr & 24'hF000FF) == 24'hC00003) begin
      matched = 1'b1;
      tag0    = UOP_ST128_MASK;
      count   = 2'd1;
    end
  endtask

endpackage : decode_table_pkg
