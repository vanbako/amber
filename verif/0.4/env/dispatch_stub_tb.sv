`timescale 1ns/1ps

module dispatch_stub_tb;

  import uop_pkg::*;
  import cluster_pkg::*;

  localparam int unsigned MAX_UOPS_WIDE   = 2;
  localparam int unsigned MAX_UOPS_NARROW = 1;

  logic clk;
  logic rst_n;

  logic              rename_valid;
  uop_tag_t          rename_uop0;
  uop_tag_t          rename_uop1;
  logic [1:0]        rename_uop_count;

  logic              dispatch_ready_wide;
  logic [MAX_UOPS_WIDE*2-1:0] lane_cluster_wide;
  logic [15:0]       alu_issue_count_wide;
  logic [15:0]       capability_issue_count_wide;
  logic [15:0]       lsq_issue_count_wide;
  logic [15:0]       async_issue_count_wide;

  logic              dispatch_ready_narrow;
  logic [MAX_UOPS_NARROW*2-1:0] lane_cluster_narrow;
  logic [15:0]       alu_issue_count_narrow;
  logic [15:0]       capability_issue_count_narrow;
  logic [15:0]       lsq_issue_count_narrow;
  logic [15:0]       async_issue_count_narrow;

  dispatch_stub #(
    .MAX_UOPS(MAX_UOPS_WIDE)
  ) i_dispatch_wide (
    .clk_i                    (clk),
    .rst_ni                   (rst_n),
    .rename_valid_i           (rename_valid),
    .rename_uop0_i            (rename_uop0),
    .rename_uop1_i            (rename_uop1),
    .rename_uop_count_i       (rename_uop_count),
    .dispatch_ready_o         (dispatch_ready_wide),
    .lane_cluster_o           (lane_cluster_wide),
    .alu_issue_count_o        (alu_issue_count_wide),
    .capability_issue_count_o (capability_issue_count_wide),
    .lsq_issue_count_o        (lsq_issue_count_wide),
    .async_issue_count_o      (async_issue_count_wide)
  );

  dispatch_stub #(
    .MAX_UOPS(MAX_UOPS_NARROW)
  ) i_dispatch_narrow (
    .clk_i                    (clk),
    .rst_ni                   (rst_n),
    .rename_valid_i           (rename_valid),
    .rename_uop0_i            (rename_uop0),
    .rename_uop1_i            (rename_uop1),
    .rename_uop_count_i       (rename_uop_count),
    .dispatch_ready_o         (dispatch_ready_narrow),
    .lane_cluster_o           (lane_cluster_narrow),
    .alu_issue_count_o        (alu_issue_count_narrow),
    .capability_issue_count_o (capability_issue_count_narrow),
    .lsq_issue_count_o        (lsq_issue_count_narrow),
    .async_issue_count_o      (async_issue_count_narrow)
  );

  function automatic cluster_sel_e lane_cluster_at_wide(int unsigned idx);
    return lane_cluster_wide[idx*2 +: 2];
  endfunction

  always begin
    clk = 1'b0;
    #5;
    clk = 1'b1;
    #5;
  end

  initial begin
    rst_n            = 1'b0;
    rename_valid     = 1'b0;
    rename_uop0      = UOP_INT_ALU;
    rename_uop1      = UOP_INT_ALU;
    rename_uop_count = 2'd0;

    repeat (2) @(posedge clk);
    rst_n = 1'b1;
    @(posedge clk);
    #1;
    assert (lane_cluster_wide == '0);
    assert (alu_issue_count_wide == 16'd0);
    assert (capability_issue_count_wide == 16'd0);
    assert (lsq_issue_count_wide == 16'd0);
    assert (async_issue_count_wide == 16'd0);
    assert (lane_cluster_narrow == '0);
    assert (alu_issue_count_narrow == 16'd0);
    assert (capability_issue_count_narrow == 16'd0);
    assert (lsq_issue_count_narrow == 16'd0);
    assert (async_issue_count_narrow == 16'd0);
    assert (dispatch_ready_wide && dispatch_ready_narrow)
      else $fatal(1, "Dispatch should be ready after reset");

    // Zero count handshake: should default clusters and hold counters.
    drive_dispatch(1'b1, 2'd0, UOP_CAP_RET, UOP_CAP_RET);
    #1;
    assert (lane_cluster_at_wide(0) == CLUSTER_ALU)
      else $fatal(1, "Lane0 should default to ALU when count zero");
    assert (lane_cluster_at_wide(1) == CLUSTER_ALU)
      else $fatal(1, "Lane1 should default to ALU when count zero");
    assert (lane_cluster_narrow == CLUSTER_ALU)
      else $fatal(1, "Narrow lane should default to ALU when count zero");
    assert (alu_issue_count_wide == 16'd0);
    assert (capability_issue_count_wide == 16'd0);
    assert (lsq_issue_count_wide == 16'd0);
    assert (async_issue_count_wide == 16'd0);

    // ALU classification.
    drive_dispatch(1'b1, 2'd1, UOP_INT_SAT_ADD, UOP_INT_ALU);
    #1;
    assert (lane_cluster_at_wide(0) == CLUSTER_ALU);
    assert (lane_cluster_at_wide(1) == CLUSTER_ALU);
    assert (lane_cluster_narrow == CLUSTER_ALU);
    assert (alu_issue_count_wide == 16'd1);
    assert (alu_issue_count_narrow == 16'd1);

    // LSQ classification.
    drive_dispatch(1'b1, 2'd1, UOP_LD_U8, UOP_INT_ALU);
    #1;
    assert (lane_cluster_at_wide(0) == CLUSTER_LSQ);
    assert (lane_cluster_at_wide(1) == CLUSTER_ALU);
    assert (lane_cluster_narrow == CLUSTER_LSQ);
    assert (lsq_issue_count_wide == 16'd1);
    assert (lsq_issue_count_narrow == 16'd1);

    // Capability classification.
    drive_dispatch(1'b1, 2'd1, UOP_CAP_LOAN_BEGIN, UOP_INT_ALU);
    #1;
    assert (lane_cluster_at_wide(0) == CLUSTER_CAPABILITY);
    assert (lane_cluster_at_wide(1) == CLUSTER_ALU);
    assert (lane_cluster_narrow == CLUSTER_CAPABILITY);
    assert (capability_issue_count_wide == 16'd1);
    assert (capability_issue_count_narrow == 16'd1);

    // Async classification.
    drive_dispatch(1'b1, 2'd1, UOP_MEM_PREFETCH, UOP_INT_ALU);
    #1;
    assert (lane_cluster_at_wide(0) == CLUSTER_ASYNC);
    assert (lane_cluster_at_wide(1) == CLUSTER_ALU);
    assert (lane_cluster_narrow == CLUSTER_ASYNC);
    assert (async_issue_count_wide == 16'd1);
    assert (async_issue_count_narrow == 16'd1);

    // Two-lane handshake with different clusters.
    drive_dispatch(1'b1, 2'd2, UOP_CAP_JUMP, UOP_LD_U8);
    #1;
    assert (lane_cluster_at_wide(0) == CLUSTER_CAPABILITY);
    assert (lane_cluster_at_wide(1) == CLUSTER_LSQ);
    assert (lane_cluster_narrow == CLUSTER_CAPABILITY);
    assert (capability_issue_count_wide == 16'd2);
    assert (lsq_issue_count_wide == 16'd2);
    assert (capability_issue_count_narrow == 16'd2);
    assert (lsq_issue_count_narrow == 16'd1);

    // Default path (unknown tag) should map to ALU.
    drive_dispatch(1'b1, 2'd1, 8'hFF, UOP_INT_ALU);
    #1;
    assert (lane_cluster_at_wide(0) == CLUSTER_ALU);
    assert (lane_cluster_at_wide(1) == CLUSTER_ALU);
    assert (lane_cluster_narrow == CLUSTER_ALU);
    assert (alu_issue_count_wide == 16'd2);
    assert (alu_issue_count_narrow == 16'd2);

    // No handshake cycle keeps state.
    rename_valid = 1'b0;
    rename_uop0  = UOP_LD_U8;
    rename_uop_count = 2'd1;
    @(posedge clk);
    #1;
    assert (lane_cluster_at_wide(0) == CLUSTER_ALU);
    assert (lane_cluster_at_wide(1) == CLUSTER_ALU);
    assert (lane_cluster_narrow == CLUSTER_ALU);
    assert (alu_issue_count_wide == 16'd2);
    assert (lsq_issue_count_wide == 16'd2);
    assert (capability_issue_count_wide == 16'd2);
    assert (async_issue_count_wide == 16'd1);
    assert (alu_issue_count_narrow == 16'd2);
    assert (lsq_issue_count_narrow == 16'd1);
    assert (capability_issue_count_narrow == 16'd2);
    assert (async_issue_count_narrow == 16'd1);

    $finish;
  end

  task automatic drive_dispatch(
    input logic       valid,
    input logic [1:0] count,
    input uop_tag_t   uop0,
    input uop_tag_t   uop1
  );
    rename_valid     = valid;
    rename_uop_count = count;
    rename_uop0      = uop0;
    rename_uop1      = uop1;
    @(posedge clk);
    rename_valid     = 1'b0;
    rename_uop_count = 2'd0;
    rename_uop0      = UOP_INT_ALU;
    rename_uop1      = UOP_INT_ALU;
  endtask

endmodule : dispatch_stub_tb

