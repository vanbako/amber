// Amber v0.4 micro-op tag definitions.
// Mirrors the encodings captured in doc/0.4/implementation/0.4_impl_overview.md.

package uop_pkg;

  typedef logic [7:0] uop_tag_t;

  // ALU cluster micro-ops
  localparam uop_tag_t UOP_INT_ALU       = 8'h00;
  localparam uop_tag_t UOP_INT_SHIFT     = 8'h01;
  localparam uop_tag_t UOP_INT_BRANCH    = 8'h02;
  localparam uop_tag_t UOP_INT_SAT_ADD   = 8'h08;
  localparam uop_tag_t UOP_INT_SAT_SUB   = 8'h09;

  // SIMD / packed variants that execute within the ALU slice
  localparam uop_tag_t UOP_PACK_ADD_SAT  = 8'h10;
  localparam uop_tag_t UOP_PACK_SUB_SAT  = 8'h11;
  localparam uop_tag_t UOP_PACK_AVG      = 8'h12;
  localparam uop_tag_t UOP_PACK_MINMAX   = 8'h13;

  // Prefix and capability management micro-ops
  localparam uop_tag_t UOP_PREFIX_SELECT      = 8'h20;
  localparam uop_tag_t UOP_PREFIX_CANCEL      = 8'h21;
  localparam uop_tag_t UOP_CAP_CLONE_RESTRICT = 8'h28;
  localparam uop_tag_t UOP_CAP_LOAN_BEGIN     = 8'h29;
  localparam uop_tag_t UOP_CAP_LOAN_END       = 8'h2A;
  localparam uop_tag_t UOP_CAP_JUMP           = 8'h2C;
  localparam uop_tag_t UOP_CAP_RET            = 8'h2D;
  localparam uop_tag_t UOP_LINK               = 8'h2E;

  // Load/store and prefetch micro-ops
  localparam uop_tag_t UOP_LD_U8           = 8'h30;
  localparam uop_tag_t UOP_ST_U8           = 8'h31;
  localparam uop_tag_t UOP_MEM_PREFETCH    = 8'h34;
  localparam uop_tag_t UOP_CAP_FENCE_PREFIX = 8'h35;

  // Atomic and CSR operations
  localparam uop_tag_t UOP_LR128        = 8'h38;
  localparam uop_tag_t UOP_SC128        = 8'h39;
  localparam uop_tag_t UOP_CAS128       = 8'h3A;
  localparam uop_tag_t UOP_ST128_MASK   = 8'h3B;
  localparam uop_tag_t UOP_CSR_ALIAS_RD = 8'h3C;

endpackage : uop_pkg
