`timescale 1ns/1ps

module decode_top_tb;

  import uop_pkg::*;

  localparam int unsigned MAX_UOPS_WIDE  = 2;
  localparam int unsigned MAX_UOPS_NARROW = 1;

  localparam logic [23:0] LONG_CAP_JUMP_PATTERN = 24'h400040;

  logic [23:0] instr;
  logic        instr_is_long;

  uop_tag_t wide_tag0, wide_tag1;
  logic [1:0] wide_count;
  logic       wide_match;

  uop_tag_t narrow_tag0, narrow_tag1;
  logic [1:0] narrow_count;
  logic       narrow_match;

  decode_top #(
    .MAX_UOPS(MAX_UOPS_WIDE)
  ) i_decode_wide (
    .instr_i         (instr),
    .instr_is_long_i (instr_is_long),
    .uop_tag0_o      (wide_tag0),
    .uop_tag1_o      (wide_tag1),
    .uop_count_o     (wide_count),
    .match_valid_o   (wide_match)
  );

  decode_top #(
    .MAX_UOPS(MAX_UOPS_NARROW)
  ) i_decode_narrow (
    .instr_i         (instr),
    .instr_is_long_i (instr_is_long),
    .uop_tag0_o      (narrow_tag0),
    .uop_tag1_o      (narrow_tag1),
    .uop_count_o     (narrow_count),
    .match_valid_o   (narrow_match)
  );

  initial begin
    instr         = '0;
    instr_is_long = 1'b0;

    // Short instruction coverage
    check_short_match(12'hE00, UOP_PREFIX_SELECT);
    check_short_match(12'hE07, UOP_PREFIX_CANCEL);
    check_short_match(12'hA00, UOP_INT_SAT_ADD);
    check_short_match(12'hA40, UOP_INT_SAT_ADD); // vary masked bits, still matches add
    check_short_match(12'hA11, UOP_INT_SAT_SUB);
    check_short_match(12'h800, UOP_LD_U8);
    check_short_match(12'h820, UOP_LD_U8); // vary masked bits, still matches load
    check_short_match(12'h801, UOP_ST_U8);
    check_short_match(12'hC02, UOP_CSR_ALIAS_RD);
    check_short_miss(12'h123);

    // Ensure short encoding treated as miss when flagged long
    check_long_miss({12'h000, 12'hE00});

    // Long instruction coverage
    check_long_match(24'h500000, UOP_PACK_ADD_SAT);
    check_long_match(24'h500010, UOP_PACK_SUB_SAT);
    check_long_match(24'h500020, UOP_PACK_AVG);
    check_long_match(24'h500030, UOP_PACK_MINMAX);
    check_long_match(24'h600000, UOP_MEM_PREFETCH);
    check_long_match(24'h700000, UOP_CAP_FENCE_PREFIX);
    check_long_match(24'h800123, UOP_CAP_CLONE_RESTRICT);
    check_long_match(24'h900FF1, UOP_CAP_LOAN_END);
    check_long_match(24'h900ABC, UOP_CAP_LOAN_BEGIN);
    check_long_match(24'h300001, UOP_INT_BRANCH);
    check_long_dual (24'h400040, UOP_CAP_JUMP, UOP_LINK);
    check_long_match(24'h200004, UOP_CAP_JUMP);
    check_long_match(24'h200001, UOP_CAP_RET);
    check_long_match(24'hC00000, UOP_LR128);
    check_long_match(24'hC00001, UOP_SC128);
    check_long_match(24'hC00002, UOP_CAS128);
    check_long_match(24'hC00003, UOP_ST128_MASK);
    check_long_miss(24'h012345);

    // Ensure long encoding ignored when treated as short
    check_short_miss(LONG_CAP_JUMP_PATTERN[11:0]);

    $finish;
  end

  task automatic check_short_match(
    input logic [11:0] short_instr,
    input uop_tag_t    expected_tag0
  );
    instr         = {12'h000, short_instr};
    instr_is_long = 1'b0;
    #1;
    assert (wide_match) else $fatal(1, "Short decode miss for instr %h", short_instr);
    assert (wide_tag0 == expected_tag0)
      else $fatal(1, "Short decode tag mismatch %h expected %h", wide_tag0, expected_tag0);
    assert (wide_count == 2'd1)
      else $fatal(1, "Short decode count mismatch %0d", wide_count);
    assert (narrow_match == wide_match)
      else $fatal(1, "Narrow decoder mismatch on short instr %h", short_instr);
    assert (narrow_tag0 == expected_tag0)
      else $fatal(1, "Narrow decode tag mismatch %h expected %h", narrow_tag0, expected_tag0);
    assert (narrow_count == wide_count)
      else $fatal(1, "Narrow decode count mismatch %0d expected %0d", narrow_count, wide_count);
    assert (wide_tag1 == UOP_INT_ALU)
      else $fatal(1, "Unexpected tag1 for single-uop short instr %h", short_instr);
    assert (narrow_tag1 == UOP_INT_ALU)
      else $fatal(1, "Narrow decoder tag1 not forced to INT_ALU for short instr %h", short_instr);
  endtask

  task automatic check_short_miss(input logic [11:0] short_instr);
    instr         = {12'h000, short_instr};
    instr_is_long = 1'b0;
    #1;
    assert (!wide_match)
      else $fatal(1, "Unexpected short decode match for instr %h", short_instr);
    assert (wide_count == 2'd0)
      else $fatal(1, "Short miss should keep count zero (got %0d)", wide_count);
    assert (!narrow_match)
      else $fatal(1, "Narrow decoder produced match for miss instr %h", short_instr);
    assert (narrow_count == 2'd0)
      else $fatal(1, "Narrow miss should keep count zero (got %0d)", narrow_count);
  endtask

  task automatic check_long_match(
    input logic [23:0] long_instr,
    input uop_tag_t    expected_tag0
  );
    instr         = long_instr;
    instr_is_long = 1'b1;
    #1;
    assert (wide_match)
      else $fatal(1, "Long decode miss for instr %h", long_instr);
    assert (wide_tag0 == expected_tag0)
      else $fatal(1, "Long decode tag0 mismatch %h expected %h", wide_tag0, expected_tag0);
    assert (wide_count == 2'd1)
      else $fatal(1, "Long decode count mismatch %0d expected 1", wide_count);
    assert (narrow_match == wide_match)
      else $fatal(1, "Narrow decoder mismatch on long instr %h", long_instr);
    assert (narrow_tag0 == expected_tag0)
      else $fatal(1, "Narrow long decode tag mismatch %h expected %h", narrow_tag0, expected_tag0);
    assert (narrow_count == wide_count)
      else $fatal(1, "Narrow long decode count mismatch %0d expected %0d",
                  narrow_count, wide_count);
    assert (wide_tag1 == UOP_INT_ALU)
      else $fatal(1, "Unexpected tag1 for single-uop long instr %h", long_instr);
    assert (narrow_tag1 == UOP_INT_ALU)
      else $fatal(1, "Narrow decoder tag1 not forced to INT_ALU for long instr %h", long_instr);
  endtask

  task automatic check_long_dual(
    input logic [23:0] long_instr,
    input uop_tag_t    expected_tag0,
    input uop_tag_t    expected_tag1
  );
    instr         = long_instr;
    instr_is_long = 1'b1;
    #1;
    assert (wide_match)
      else $fatal(1, "Dual-uop long decode miss for instr %h", long_instr);
    assert (wide_tag0 == expected_tag0)
      else $fatal(1, "Dual-uop long decode tag0 mismatch %h expected %h",
                  wide_tag0, expected_tag0);
    assert (wide_tag1 == expected_tag1)
      else $fatal(1, "Dual-uop long decode tag1 mismatch %h expected %h",
                  wide_tag1, expected_tag1);
    assert (wide_count == 2'd2)
      else $fatal(1, "Dual-uop long count mismatch %0d expected 2", wide_count);
    assert (narrow_match == wide_match)
      else $fatal(1, "Narrow decoder mismatch on dual-uop instr %h", long_instr);
    assert (narrow_tag0 == expected_tag0)
      else $fatal(1, "Narrow long decode tag0 mismatch %h expected %h",
                  narrow_tag0, expected_tag0);
    assert (narrow_tag1 == UOP_INT_ALU)
      else $fatal(1, "Narrow decoder tag1 should default for dual-uop instr %h", long_instr);
    assert (narrow_count == wide_count)
      else $fatal(1, "Narrow dual-uop count mismatch %0d expected %0d",
                  narrow_count, wide_count);
  endtask

  task automatic check_long_miss(input logic [23:0] long_instr);
    instr         = long_instr;
    instr_is_long = 1'b1;
    #1;
    assert (!wide_match)
      else $fatal(1, "Unexpected long decode match for instr %h", long_instr);
    assert (wide_count == 2'd0)
      else $fatal(1, "Long miss should keep count zero (got %0d)", wide_count);
    assert (!narrow_match)
      else $fatal(1, "Narrow decoder produced match for miss instr %h", long_instr);
    assert (narrow_count == 2'd0)
      else $fatal(1, "Narrow long miss should keep count zero (got %0d)", narrow_count);
  endtask

endmodule : decode_top_tb
