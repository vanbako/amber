`timescale 1ns/1ps

// A-bank: 8 x 48, index 0 hard-wired to zero (reads as 0, writes ignored).
module regfileA(
  input              clk,
  input       [2:0]  raddr,   // rsA
  output      [47:0] rdata,
  input              we,
  input       [2:0]  waddr,
  input       [47:0] wdata
);
  reg [47:0] regs[0:7];

  integer idx;
  initial begin
    for (idx = 0; idx < 8; idx = idx + 1) begin
      regs[idx] = 48'd0;
    end
  end

  assign rdata = (raddr == 3'd0) ? 48'd0 : regs[raddr];
  always @(posedge clk) begin
    if (we && (waddr != 3'd0)) regs[waddr] <= wdata;
  end
endmodule

// D-bank: 8 x 48, all writable.
module regfileD(
  input              clk,
  input       [2:0]  raddr,   // rsD
  output      [47:0] rdata,
  input              we,
  input       [2:0]  waddr,
  input       [47:0] wdata
);
  reg [47:0] regs[0:7];

  integer j;
  initial begin
    for (j = 0; j < 8; j = j + 1) begin
      regs[j] = 48'd0;
    end
  end

  assign rdata = regs[raddr];
  always @(posedge clk) begin
    if (we) regs[waddr] <= wdata;
  end
endmodule
