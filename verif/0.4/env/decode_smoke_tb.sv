`timescale 1ns/1ps

module decode_smoke_tb;

  import uop_pkg::*;
  import decode_table_pkg::*;
  import cluster_pkg::*;

  localparam int unsigned MAX_UOPS = 2;

  logic clk;
  logic rst_n;

  logic [23:0] instr;
  logic        instr_is_long;
  uop_tag_t    uop_tag0;
  uop_tag_t    uop_tag1;
  logic [1:0]  uop_count;
  logic        match_valid;

  logic              rename_ready;
  logic [MAX_UOPS-1:0] lane_is_capability;
  logic [15:0]       capability_issued_count;
  logic              dispatch_ready;
  logic              dispatch_valid;
  uop_tag_t          dispatch_uop0;
  uop_tag_t          dispatch_uop1;
  logic [1:0]        dispatch_uop_count;
  logic [MAX_UOPS*2-1:0] lane_cluster;
  logic [15:0]       alu_issue_count;
  logic [15:0]       capability_cluster_issue_count;
  logic [15:0]       lsq_issue_count;
  logic [15:0]       async_issue_count;

  function automatic logic [1:0] lane_cluster_at(int unsigned lane_idx);
    return lane_cluster[lane_idx*2 +: 2];
  endfunction

  decode_top #(
    .MAX_UOPS(MAX_UOPS)
  ) i_decode_top (
    .instr_i         (instr),
    .instr_is_long_i (instr_is_long),
    .uop_tag0_o      (uop_tag0),
    .uop_tag1_o      (uop_tag1),
    .uop_count_o     (uop_count),
    .match_valid_o   (match_valid)
  );

  rename_stub #(
    .MAX_UOPS(MAX_UOPS)
  ) i_rename_stub (
    .clk_i                    (clk),
    .rst_ni                   (rst_n),
    .decode_valid_i           (match_valid),
    .decode_uop0_i            (uop_tag0),
    .decode_uop1_i            (uop_tag1),
    .decode_uop_count_i       (uop_count),
    .dispatch_ready_i         (dispatch_ready),
    .rename_ready_o           (rename_ready),
    .dispatch_valid_o         (dispatch_valid),
    .dispatch_uop0_o          (dispatch_uop0),
    .dispatch_uop1_o          (dispatch_uop1),
    .dispatch_uop_count_o     (dispatch_uop_count),
    .lane_is_capability_o     (lane_is_capability),
    .capability_issued_count_o(capability_issued_count)
  );

  dispatch_stub #(
    .MAX_UOPS(MAX_UOPS)
  ) i_dispatch_stub (
    .clk_i                    (clk),
    .rst_ni                   (rst_n),
    .rename_valid_i           (dispatch_valid),
    .rename_uop0_i            (dispatch_uop0),
    .rename_uop1_i            (dispatch_uop1),
    .rename_uop_count_i       (dispatch_uop_count),
    .dispatch_ready_o         (dispatch_ready),
    .lane_cluster_o           (lane_cluster),
    .alu_issue_count_o        (alu_issue_count),
    .capability_issue_count_o (capability_cluster_issue_count),
    .lsq_issue_count_o        (lsq_issue_count),
    .async_issue_count_o      (async_issue_count)
  );

  initial begin
    clk = 1'b0;
    forever #5 clk = ~clk;
  end

  initial begin
    rst_n = 1'b0;
    instr = '0;
    instr_is_long = 1'b0;
    @(posedge clk);
    rst_n = 1'b1;

    drive_short_instruction(12'hEE8, UOP_PREFIX_SELECT, 1'b1, CLUSTER_CAPABILITY); // cap_prefix_select
    drive_short_instruction(12'h895, UOP_ST_U8, 1'b0, CLUSTER_LSQ);               // mem_store_u8

    drive_long_instruction(24'h500000, UOP_PACK_ADD_SAT, 1'b0, CLUSTER_ALU); // pack_add_sat
    drive_long_instruction(24'h400040, UOP_CAP_JUMP, 1'b1, CLUSTER_CAPABILITY,
                           2'd2, UOP_LINK, 1'b1, CLUSTER_CAPABILITY);        // call_compact with immediate
    drive_long_instruction(24'h900FF1, UOP_CAP_LOAN_END, 1'b1, CLUSTER_CAPABILITY); // capability_loan_end
    drive_long_instruction(24'h600000, UOP_MEM_PREFETCH, 1'b0, CLUSTER_ASYNC); // mem_prefetch (async cluster)
    drive_long_instruction(24'hC00000, UOP_LR128, 1'b0, CLUSTER_LSQ); // atomic_load_reserved_128 (LSQ cluster)

    @(posedge clk);
    assert (capability_issued_count == 16'd4)
      else $fatal(1, "Capability issued count mismatch (got %0d)", capability_issued_count);
    assert (capability_cluster_issue_count == 16'd4)
      else $fatal(1, "Dispatch capability count mismatch (got %0d)", capability_cluster_issue_count);
    assert (lsq_issue_count == 16'd2)
      else $fatal(1, "Dispatch LSQ count mismatch (got %0d)", lsq_issue_count);
    assert (alu_issue_count == 16'd1)
      else $fatal(1, "Dispatch ALU count mismatch (got %0d)", alu_issue_count);
    assert (async_issue_count == 16'd1)
      else $fatal(1, "Dispatch async count mismatch (got %0d)", async_issue_count);
    $finish;
  end

  task automatic drive_short_instruction(
    input logic [11:0] short_instr,
    input uop_tag_t    expected_uop,
    input logic        expected_cap_lane,
    input cluster_sel_e expected_cluster
  );
    instr         = {12'h000, short_instr};
    instr_is_long = 1'b0;
    #1;
    assert (rename_ready) else $fatal(1, "Rename not ready for short instr %h", short_instr);
    assert (match_valid) else $fatal(1, "Short decode failed to match instr %h", short_instr);
    assert (uop_count == 2'd1) else $fatal(1, "Short decode produced unexpected uop count %0d", uop_count);
    assert (uop_tag0 == expected_uop)
      else $fatal(1, "Short decode produced uop %h expected %h", uop_tag0, expected_uop);
    @(posedge clk);
    #1;
    assert (lane_is_capability[0] == expected_cap_lane)
      else $fatal(1, "Rename lane classification mismatch for short instr %h", short_instr);
    assert (lane_cluster_at(0) == expected_cluster)
      else $fatal(1, "Dispatch cluster mismatch for short instr %h (got %0d exp %0d)",
                  short_instr, lane_cluster_at(0), expected_cluster);
    instr         = 24'hFFFFFF;
    instr_is_long = 1'b0;
    @(posedge clk);
  endtask

  task automatic drive_long_instruction(
    input logic [23:0] long_instr,
    input uop_tag_t    expected_uop0,
    input logic        expected_cap_lane0,
    input cluster_sel_e expected_cluster0,
    input logic [1:0]  expected_uop_count = 2'd1,
    input uop_tag_t    expected_uop1 = UOP_INT_ALU,
    input logic        expected_cap_lane1 = 1'b0,
    input cluster_sel_e expected_cluster1 = CLUSTER_ALU
  );
    instr         = long_instr;
    instr_is_long = 1'b1;
    #1;
    assert (rename_ready) else $fatal(1, "Rename not ready for long instr %h", long_instr);
    assert (match_valid) else $fatal(1, "Long decode failed to match instr %h", long_instr);
    assert (uop_count == expected_uop_count)
      else $fatal(1, "Long decode count mismatch for instr %h (got %0d, expected %0d)",
                   long_instr, uop_count, expected_uop_count);
    assert (uop_tag0 == expected_uop0)
      else $fatal(1, "Long decode produced uop0 %h expected %h", uop_tag0, expected_uop0);
    if (expected_uop_count > 1 && expected_uop1 != UOP_INT_ALU) begin
      assert (uop_tag1 == expected_uop1)
        else $fatal(1, "Long decode produced uop1 %h expected %h", uop_tag1, expected_uop1);
    end
    @(posedge clk);
    #1;
    assert (lane_is_capability[0] == expected_cap_lane0)
      else $fatal(1, "Rename lane0 capability mismatch for long instr %h", long_instr);
    assert (lane_cluster_at(0) == expected_cluster0)
      else $fatal(1, "Dispatch lane0 cluster mismatch for long instr %h (got %0d exp %0d)",
                  long_instr, lane_cluster_at(0), expected_cluster0);
    if (expected_uop_count > 1) begin
      assert (lane_is_capability[1] == expected_cap_lane1)
        else $fatal(1, "Rename lane1 capability mismatch for long instr %h", long_instr);
      assert (lane_cluster_at(1) == expected_cluster1)
        else $fatal(1, "Dispatch lane1 cluster mismatch for long instr %h (got %0d exp %0d)",
                    long_instr, lane_cluster_at(1), expected_cluster1);
    end
    instr         = 24'hFFFFFF;
    instr_is_long = 1'b1;
    @(posedge clk);
  endtask

endmodule : decode_smoke_tb
