// Amber v0.4 dispatch stub.
// Classifies rename micro-ops into scheduler clusters and accumulates per-cluster counts.

module dispatch_stub #(
  parameter int unsigned MAX_UOPS = 2
) (
  input  logic              clk_i,
  input  logic              rst_ni,
  input  logic              rename_valid_i,
  input  uop_pkg::uop_tag_t rename_uop0_i,
  input  uop_pkg::uop_tag_t rename_uop1_i,
  input  logic [1:0]        rename_uop_count_i,
  output logic              dispatch_ready_o,
  output logic [MAX_UOPS*2-1:0] lane_cluster_o,
  output logic [15:0]       alu_issue_count_o,
  output logic [15:0]       capability_issue_count_o,
  output logic [15:0]       lsq_issue_count_o,
  output logic [15:0]       async_issue_count_o
);

  import uop_pkg::*;
  import cluster_pkg::*;

  logic [MAX_UOPS*2-1:0] lane_cluster_d;
  localparam int LANE0_BASE = 0;
  localparam int LANE1_BASE = (MAX_UOPS > 1) ? 2 : 0;
  localparam bit HAS_LANE1   = (MAX_UOPS > 1);
  logic [15:0]  alu_issue_count_q;
  logic [15:0]  alu_issue_count_d;
  logic [15:0]  capability_issue_count_q;
  logic [15:0]  capability_issue_count_d;
  logic [15:0]  lsq_issue_count_q;
  logic [15:0]  lsq_issue_count_d;
  logic [15:0]  async_issue_count_q;
  logic [15:0]  async_issue_count_d;
  logic         accept_fire;

  assign dispatch_ready_o          = 1'b1;
  assign alu_issue_count_o         = alu_issue_count_q;
  assign capability_issue_count_o  = capability_issue_count_q;
  assign lsq_issue_count_o         = lsq_issue_count_q;
  assign async_issue_count_o       = async_issue_count_q;
  assign accept_fire               = rename_valid_i && dispatch_ready_o;

  function automatic cluster_sel_e classify_uop(uop_tag_t tag);
    case (tag)
      UOP_INT_ALU,
      UOP_INT_SHIFT,
      UOP_INT_BRANCH,
      UOP_INT_SAT_ADD,
      UOP_INT_SAT_SUB,
      UOP_PACK_ADD_SAT,
      UOP_PACK_SUB_SAT,
      UOP_PACK_AVG,
      UOP_PACK_MINMAX,
      UOP_CSR_ALIAS_RD: return CLUSTER_ALU;
      UOP_PREFIX_SELECT,
      UOP_PREFIX_CANCEL,
      UOP_CAP_CLONE_RESTRICT,
      UOP_CAP_LOAN_BEGIN,
      UOP_CAP_LOAN_END,
      UOP_CAP_JUMP,
      UOP_CAP_RET,
      UOP_LINK: return CLUSTER_CAPABILITY;
      UOP_LD_U8,
      UOP_ST_U8,
      UOP_LR128,
      UOP_SC128,
      UOP_CAS128,
      UOP_ST128_MASK: return CLUSTER_LSQ;
      UOP_MEM_PREFETCH,
      UOP_CAP_FENCE_PREFIX: return CLUSTER_ASYNC;
      default: return CLUSTER_ALU;
    endcase
  endfunction

  task automatic bump_cluster_counts(input cluster_sel_e cluster);
    case (cluster)
      CLUSTER_ALU:        alu_issue_count_d++;
      CLUSTER_CAPABILITY: capability_issue_count_d++;
      CLUSTER_LSQ:        lsq_issue_count_d++;
      CLUSTER_ASYNC:      async_issue_count_d++;
      default:            alu_issue_count_d++;
    endcase
  endtask

  always_comb begin
    lane_cluster_d           = lane_cluster_o;
    alu_issue_count_d        = alu_issue_count_q;
    capability_issue_count_d = capability_issue_count_q;
    lsq_issue_count_d        = lsq_issue_count_q;
    async_issue_count_d      = async_issue_count_q;

    if (accept_fire) begin
      if (MAX_UOPS > 0) begin
        if (rename_uop_count_i > 0) begin
          cluster_sel_e cluster0;
          cluster0 = classify_uop(rename_uop0_i);
          lane_cluster_d[LANE0_BASE +: 2] = cluster0;
          bump_cluster_counts(cluster0);
        end else begin
          lane_cluster_d[LANE0_BASE +: 2] = CLUSTER_ALU;
        end
      end

      if (HAS_LANE1) begin
        if (rename_uop_count_i > 1) begin
          cluster_sel_e cluster1;
          cluster1 = classify_uop(rename_uop1_i);
          lane_cluster_d[LANE1_BASE +: 2] = cluster1;
          bump_cluster_counts(cluster1);
        end else begin
          lane_cluster_d[LANE1_BASE +: 2] = CLUSTER_ALU;
        end
      end
    end
  end

  always_ff @(posedge clk_i or negedge rst_ni) begin
    if (!rst_ni) begin
      lane_cluster_o            <= '0;
      alu_issue_count_q        <= '0;
      capability_issue_count_q <= '0;
      lsq_issue_count_q        <= '0;
      async_issue_count_q      <= '0;
    end else begin
      lane_cluster_o            <= lane_cluster_d;
      alu_issue_count_q        <= alu_issue_count_d;
      capability_issue_count_q <= capability_issue_count_d;
      lsq_issue_count_q        <= lsq_issue_count_d;
      async_issue_count_q      <= async_issue_count_d;
    end
  end

endmodule : dispatch_stub
