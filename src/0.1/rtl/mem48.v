// Simple 48-bit word memory with synchronous write / combinational read.
module simple_mem48 #(
  parameter WORDS = 16384
)(
  input                             clk,
  input       [$clog2(WORDS)-1:0]   addr,   // word index
  input                             we,
  input       [47:0]                wdata,
  output reg  [47:0]                rdata
);
  reg [47:0] mem [0:WORDS-1];

  integer i;
  initial begin
    for (i = 0; i < WORDS; i = i + 1) begin
      mem[i] = 48'd0;
    end
  end

  always @(posedge clk) begin
    if (we) mem[addr] <= wdata;
  end

  always @* begin
    rdata = mem[addr];
  end
  // $readmemh("image.hex", mem); // instantiate separately for IMEM/DMEM
endmodule
