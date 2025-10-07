// Amber v0.4 rename stub.
// Categorises micro-ops using uop_pkg encodings and tracks capability traffic.

module rename_stub #(
  parameter int unsigned MAX_UOPS = 2
) (
  input  logic              clk_i,
  input  logic              rst_ni,
  input  logic              decode_valid_i,
  input  uop_pkg::uop_tag_t decode_uop0_i,
  input  uop_pkg::uop_tag_t decode_uop1_i,
  input  logic [1:0]        decode_uop_count_i,
  output logic              rename_ready_o,
  output logic [MAX_UOPS-1:0] lane_is_capability_o,
  output logic [15:0]       capability_issued_count_o
);

  import uop_pkg::*;

  logic [15:0] capability_issued_count_d;

  assign rename_ready_o = 1'b1; // Stub never back-pressures the decoder.

  function automatic logic is_capability_uop(uop_tag_t tag);
    case (tag)
      UOP_PREFIX_SELECT,
      UOP_PREFIX_CANCEL,
      UOP_CAP_CLONE_RESTRICT,
      UOP_CAP_LOAN_BEGIN,
      UOP_CAP_LOAN_END,
      UOP_CAP_JUMP,
      UOP_CAP_RET,
      UOP_LINK: return 1'b1;
      default: return 1'b0;
    endcase
  endfunction

  always_comb begin
    lane_is_capability_o       = '0;
    capability_issued_count_d  = capability_issued_count_o;

    if (decode_valid_i && rename_ready_o) begin
      for (int unsigned idx = 0; idx < MAX_UOPS; idx++) begin
        uop_pkg::uop_tag_t lane_tag;
        if (idx < decode_uop_count_i) begin
          lane_tag = (idx == 0) ? decode_uop0_i : decode_uop1_i;
          lane_is_capability_o[idx] = is_capability_uop(lane_tag);
          capability_issued_count_d += lane_is_capability_o[idx];
        end
      end
    end
  end

  always_ff @(posedge clk_i or negedge rst_ni) begin
    if (!rst_ni) begin
      capability_issued_count_o <= '0;
    end else begin
      capability_issued_count_o <= capability_issued_count_d;
    end
  end

endmodule : rename_stub
