`timescale 1ns/1ps

module amber128_capfile
  import amber128_pkg::*;
(
    input  logic                   clk_i,
    input  logic                   rst_ni,
    // 1W, 3R capability file
    input  logic                   we_i,
    input  logic [CAP_REG_AW-1:0]  waddr_i,
    input  logic [C_XLEN-1:0]      wdata_i,
    input  logic [CAP_REG_AW-1:0]  raddr0_i,
    output logic [C_XLEN-1:0]      rdata0_o,
    input  logic [CAP_REG_AW-1:0]  raddr1_i,
    output logic [C_XLEN-1:0]      rdata1_o,
    input  logic [CAP_REG_AW-1:0]  raddr2_i,
    output logic [C_XLEN-1:0]      rdata2_o
);

  logic [C_XLEN-1:0] caps [CAP_REG_COUNT-1:0];
  integer i;

  always_ff @(posedge clk_i or negedge rst_ni) begin
    if (!rst_ni) begin
      for (i = 0; i < CAP_REG_COUNT; i++) begin
        caps[i] <= '0;
      end
    end else begin
      if (we_i) begin
        caps[waddr_i] <= wdata_i;
      end
    end
  end

  assign rdata0_o = caps[raddr0_i];
  assign rdata1_o = caps[raddr1_i];
  assign rdata2_o = caps[raddr2_i];

endmodule
