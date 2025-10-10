`timescale 1ns/1ps

`include "capability_defs.vh"

// A-bank: 8 x 48, index 0 hard-wired to zero (reads as 0, writes ignored).
module regfileA(
  input                             clk,
  input        [2:0]                raddr,       // rsA
  output       [47:0]               rdata,
  output                            cap_valid,
  output       [`CAP_ADDR_WIDTH-1:0] cap_base,
  output       [`CAP_ADDR_WIDTH-1:0] cap_limit,
  output       [`CAP_PERM_WIDTH-1:0] cap_perms,
  input                             we,
  input        [2:0]                waddr,
  input        [47:0]               wdata,
  input                             cap_we,
  input        [2:0]                cap_waddr,
  input                             cap_valid_in,
  input        [`CAP_ADDR_WIDTH-1:0] cap_base_in,
  input        [`CAP_ADDR_WIDTH-1:0] cap_limit_in,
  input        [`CAP_PERM_WIDTH-1:0] cap_perms_in
);
  reg [47:0] regs[0:7];
  reg [`CAP_ADDR_WIDTH-1:0] cap_base_mem[0:7];
  reg [`CAP_ADDR_WIDTH-1:0] cap_limit_mem[0:7];
  reg [`CAP_PERM_WIDTH-1:0] cap_perms_mem[0:7];
  reg                      cap_valid_mem[0:7];

  integer idx;
  initial begin
    for (idx = 0; idx < 8; idx = idx + 1) begin
      regs[idx] = 48'd0;
      cap_base_mem[idx]  = {`CAP_ADDR_WIDTH{1'b0}};
      cap_limit_mem[idx] = {`CAP_ADDR_WIDTH{1'b1}};
      cap_perms_mem[idx] = `CAP_PERM_ALL;
      cap_valid_mem[idx] = 1'b1;
    end
  end

  assign rdata = (raddr == 3'd0) ? 48'd0 : regs[raddr];
  assign cap_base  = cap_base_mem[raddr];
  assign cap_limit = cap_limit_mem[raddr];
  assign cap_perms = cap_perms_mem[raddr];
  assign cap_valid = cap_valid_mem[raddr];

  always @(posedge clk) begin
    if (we && (waddr != 3'd0)) begin
      regs[waddr] <= wdata;
      cap_base_mem[waddr]  <= {`CAP_ADDR_WIDTH{1'b0}};
      cap_limit_mem[waddr] <= {`CAP_ADDR_WIDTH{1'b1}};
      cap_perms_mem[waddr] <= `CAP_PERM_ALL;
      cap_valid_mem[waddr] <= 1'b1;
    end
    if (cap_we) begin
      cap_base_mem[cap_waddr]  <= cap_base_in;
      cap_limit_mem[cap_waddr] <= cap_limit_in;
      cap_perms_mem[cap_waddr] <= cap_perms_in;
      cap_valid_mem[cap_waddr] <= cap_valid_in;
    end
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
