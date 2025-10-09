// 48-bit ALU, minimal ops.
// Non-commutatives assume operands as (A op D). For commutatives, a 'swap' bit
// lets control flip inputs without extra datapaths in the regfiles.
module alu48(
  input  [47:0] a_in,    // from A bank (or swapped)
  input  [47:0] b_in,    // from D bank (or swapped)
  input  [5:0]  op,      // see localparams in cpu for encoding
  input  [5:0]  shamt,   // 0..47
  output reg [47:0] y,
  output          eq,
  output          lt_s,
  output          lt_u
);
  wire signed [47:0] as = a_in;
  wire signed [47:0] bs = b_in;

  assign eq   = (a_in == b_in);
  assign lt_s = (as < bs);
  assign lt_u = (a_in < b_in);

  always @* begin
    case (op)
      6'h00: y = a_in + b_in;           // ADD
      6'h01: y = a_in - b_in;           // SUB (A - D)
      6'h02: y = a_in & b_in;           // AND
      6'h03: y = a_in | b_in;           // OR
      6'h04: y = a_in ^ b_in;           // XOR
      6'h05: y = a_in << shamt;         // SLL
      6'h06: y = a_in >> shamt;         // SRL
      6'h07: y = $signed(a_in) >>> shamt; // SRA
      6'h08: y = ~a_in;                 // NOT (unary on A)
      default: y = 48'd0;
    endcase
  end
endmodule
