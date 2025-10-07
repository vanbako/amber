// Amber v0.4 decode top-level stub.
// Uses decode_table_pkg and uop_pkg to translate ISA instructions into micro-ops.

module decode_top #(
  parameter int unsigned MAX_UOPS = 2
) (
  input  logic [23:0] instr_i,
  input  logic        instr_is_long_i,
  output uop_pkg::uop_tag_t uop_tag0_o,
  output uop_pkg::uop_tag_t uop_tag1_o,
  output logic [1:0]        uop_count_o,
  output logic              match_valid_o
);

  import uop_pkg::*;
  import decode_table_pkg::*;

  uop_tag_t short_tag0, short_tag1;
  logic [1:0] short_count;
  logic short_match;
  uop_tag_t long_tag0, long_tag1;
  logic [1:0] long_count;
  logic long_match;
  uop_tag_t tag0_d, tag1_d;
  logic [1:0] count_d;
  logic       match_d;

  logic [11:0] short_instr;
  logic [23:0] long_instr;
  assign short_instr = instr_i[11:0];
  assign long_instr  = instr_i;

  always_comb begin
    tag0_d = UOP_INT_ALU;
    tag1_d = UOP_INT_ALU;
    count_d = 2'd0;
    match_d = 1'b0;
    short_match = 1'b0;
    short_tag0  = UOP_INT_ALU;
    short_tag1  = UOP_INT_ALU;
    short_count = 2'd0;
    long_match  = 1'b0;
    long_tag0   = UOP_INT_ALU;
    long_tag1   = UOP_INT_ALU;
    long_count  = 2'd0;

    if (!instr_is_long_i) begin
      decode_short_instr(short_instr, short_match, short_tag0, short_tag1, short_count);
      if (short_match) begin
        match_d = 1'b1;
        tag0_d  = short_tag0;
        tag1_d  = short_tag1;
        count_d = (short_count == 0) ? 2'd1 : short_count;
      end
    end else begin
      decode_long_instr(long_instr, long_match, long_tag0, long_tag1, long_count);
      if (long_match) begin
        match_d = 1'b1;
        tag0_d  = long_tag0;
        tag1_d  = long_tag1;
        count_d = (long_count == 0) ? 2'd1 : long_count;
      end
    end
  end

  assign uop_tag0_o = tag0_d;
  assign uop_tag1_o = (MAX_UOPS > 1) ? tag1_d : UOP_INT_ALU;
  assign uop_count_o   = count_d;
  assign match_valid_o = match_d;

endmodule : decode_top
