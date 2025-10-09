`timescale 1ns/1ps

module rename_stub_tb;

  import uop_pkg::*;

  localparam int unsigned MAX_UOPS_WIDE   = 2;
  localparam int unsigned MAX_UOPS_NARROW = 1;

  logic clk;
  logic rst_n;

  logic              decode_valid;
  uop_tag_t          decode_uop0;
  uop_tag_t          decode_uop1;
  logic [1:0]        decode_uop_count;
  logic              dispatch_ready;

  logic              rename_ready_wide;
  logic              dispatch_valid_wide;
  uop_tag_t          dispatch_uop0_wide;
  uop_tag_t          dispatch_uop1_wide;
  logic [1:0]        dispatch_uop_count_wide;
  logic [MAX_UOPS_WIDE-1:0] lane_is_capability_wide;
  logic [15:0]       capability_issued_count_wide;

  logic              rename_ready_narrow;
  logic              dispatch_valid_narrow;
  uop_tag_t          dispatch_uop0_narrow;
  uop_tag_t          dispatch_uop1_narrow;
  logic [1:0]        dispatch_uop_count_narrow;
  logic [MAX_UOPS_NARROW-1:0] lane_is_capability_narrow;
  logic [15:0]       capability_issued_count_narrow;

  rename_stub #(
    .MAX_UOPS(MAX_UOPS_WIDE)
  ) i_rename_wide (
    .clk_i                     (clk),
    .rst_ni                    (rst_n),
    .decode_valid_i            (decode_valid),
    .decode_uop0_i             (decode_uop0),
    .decode_uop1_i             (decode_uop1),
    .decode_uop_count_i        (decode_uop_count),
    .dispatch_ready_i          (dispatch_ready),
    .rename_ready_o            (rename_ready_wide),
    .dispatch_valid_o          (dispatch_valid_wide),
    .dispatch_uop0_o           (dispatch_uop0_wide),
    .dispatch_uop1_o           (dispatch_uop1_wide),
    .dispatch_uop_count_o      (dispatch_uop_count_wide),
    .lane_is_capability_o      (lane_is_capability_wide),
    .capability_issued_count_o (capability_issued_count_wide)
  );

  rename_stub #(
    .MAX_UOPS(MAX_UOPS_NARROW)
  ) i_rename_narrow (
    .clk_i                     (clk),
    .rst_ni                    (rst_n),
    .decode_valid_i            (decode_valid),
    .decode_uop0_i             (decode_uop0),
    .decode_uop1_i             (decode_uop1),
    .decode_uop_count_i        (decode_uop_count),
    .dispatch_ready_i          (dispatch_ready),
    .rename_ready_o            (rename_ready_narrow),
    .dispatch_valid_o          (dispatch_valid_narrow),
    .dispatch_uop0_o           (dispatch_uop0_narrow),
    .dispatch_uop1_o           (dispatch_uop1_narrow),
    .dispatch_uop_count_o      (dispatch_uop_count_narrow),
    .lane_is_capability_o      (lane_is_capability_narrow),
    .capability_issued_count_o (capability_issued_count_narrow)
  );

  always begin
    clk = 1'b0;
    #5;
    clk = 1'b1;
    #5;
  end

  initial begin
    rst_n               = 1'b0;
    decode_valid        = 1'b0;
    decode_uop0         = UOP_INT_ALU;
    decode_uop1         = UOP_INT_ALU;
    decode_uop_count    = 2'd0;
    dispatch_ready      = 1'b0;

    repeat (2) @(posedge clk);
    rst_n = 1'b1;
    @(posedge clk);
    #1;
    assert (capability_issued_count_wide == 16'd0)
      else $fatal(1, "Wide rename counter did not reset");
    assert (capability_issued_count_narrow == 16'd0)
      else $fatal(1, "Narrow rename counter did not reset");

    // Idle cycle - no valid decode despite ready high.
    dispatch_ready = 1'b1;
    #1;
    assert (rename_ready_wide && rename_ready_narrow)
      else $fatal(1, "Rename ready should reflect dispatch ready during idle");
    assert (!dispatch_valid_wide && !dispatch_valid_narrow)
      else $fatal(1, "Dispatch should remain idle without decode valid");
    @(posedge clk);
    #1;
    assert (capability_issued_count_wide == 16'd0);
    assert (capability_issued_count_narrow == 16'd0);

    // Non-capability single uop handshake.
    drive_handshake(1'b1, 2'd1, UOP_LD_U8, UOP_INT_ALU);
    #1;
    assert (dispatch_valid_wide && dispatch_valid_narrow)
      else $fatal(1, "Dispatch valid missing for non-cap handshake");
    assert (dispatch_uop0_wide == UOP_LD_U8 && dispatch_uop0_narrow == UOP_LD_U8)
      else $fatal(1, "Dispatch uop0 mismatch for non-cap handshake");
    assert (dispatch_uop_count_wide == 2'd1 && dispatch_uop_count_narrow == 2'd1)
      else $fatal(1, "Dispatch count mismatch for non-cap handshake");
    assert (lane_is_capability_wide == 2'b00 && lane_is_capability_narrow == 1'b0)
      else $fatal(1, "Capability lane flags unexpected for non-cap handshake");
    @(posedge clk);
    #1;
    assert (capability_issued_count_wide == 16'd0)
      else $fatal(1, "Capability counter should remain zero after non-cap handshake");
    assert (capability_issued_count_narrow == 16'd0)
      else $fatal(1, "Narrow capability counter should remain zero");
    clear_valid();

    // Capability dual-uop handshake: both capability tags.
    drive_handshake(1'b1, 2'd2, UOP_CAP_JUMP, UOP_LINK);
    #1;
    assert (lane_is_capability_wide == 2'b11)
      else $fatal(1, "Both lanes should be capability for CAP_JUMP/LINK");
    assert (lane_is_capability_narrow == 1'b1)
      else $fatal(1, "Narrow lane0 should flag capability for CAP_JUMP");
    @(posedge clk);
    #1;
    assert (capability_issued_count_wide == 16'd2)
      else $fatal(1, "Wide capability counter expected 2 (got %0d)", capability_issued_count_wide);
    assert (capability_issued_count_narrow == 16'd1)
      else $fatal(1, "Narrow capability counter expected 1 (got %0d)", capability_issued_count_narrow);
    clear_valid();

    // Back-pressure: ready low prevents handshake.
    dispatch_ready = 1'b0;
    drive_handshake(1'b1, 2'd1, UOP_CAP_RET, UOP_INT_ALU);
    #1;
    assert (!dispatch_valid_wide && !dispatch_valid_narrow)
      else $fatal(1, "Dispatch valid should stay low when ready is low");
    assert (lane_is_capability_wide == 2'b00 && lane_is_capability_narrow == 1'b0)
      else $fatal(1, "Capability flags should remain zero without handshake");
    @(posedge clk);
    #1;
    assert (capability_issued_count_wide == 16'd2);
    assert (capability_issued_count_narrow == 16'd1);
    dispatch_ready = 1'b1;
    clear_valid();

    // Zero count even though valid should not bump counters.
    drive_handshake(1'b1, 2'd0, UOP_CAP_JUMP, UOP_CAP_RET);
    #1;
    assert (dispatch_valid_wide && dispatch_valid_narrow)
      else $fatal(1, "Dispatch valid should assert even for zero-count handshake");
    assert (lane_is_capability_wide == 2'b00 && lane_is_capability_narrow == 1'b0)
      else $fatal(1, "Capability flags should hold zero when count is zero");
    @(posedge clk);
    #1;
    assert (capability_issued_count_wide == 16'd2);
    assert (capability_issued_count_narrow == 16'd1);
    clear_valid();

    // Capability only in second lane: ensure narrow counter does not increment.
    drive_handshake(1'b1, 2'd2, UOP_LD_U8, UOP_CAP_RET);
    #1;
    assert (lane_is_capability_wide == 2'b01)
      else $fatal(1, "Wide capability flags expected 01 when only lane1 is capability");
    assert (lane_is_capability_narrow == 1'b0)
      else $fatal(1, "Narrow lane0 should be 0 when capability only on lane1");
    @(posedge clk);
    #1;
    assert (capability_issued_count_wide == 16'd3)
      else $fatal(1, "Wide capability counter expected 3 after lane1 capability");
    assert (capability_issued_count_narrow == 16'd1)
      else $fatal(1, "Narrow capability counter should not change when capability in lane1 only");
    clear_valid();

    $finish;
  end

  task automatic drive_handshake(
    input logic       valid,
    input logic [1:0] count,
    input uop_tag_t   uop0,
    input uop_tag_t   uop1
  );
    decode_valid     = valid;
    decode_uop_count = count;
    decode_uop0      = uop0;
    decode_uop1      = uop1;
  endtask

  task automatic clear_valid();
    decode_valid     = 1'b0;
    decode_uop_count = 2'd0;
    decode_uop0      = UOP_INT_ALU;
    decode_uop1      = UOP_INT_ALU;
    @(posedge clk);
    #1;
  endtask

endmodule : rename_stub_tb

