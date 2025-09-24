`timescale 1ns/1ps

module amber128_regfile
  import amber128_pkg::*;
(
    input  logic                       clk_i,
    input  logic                       rst_ni,
    input  amber128_regfile_req_s      req_i,
    input  logic                       wr_en_i,
    input  logic [DATA_REG_AW-1:0]     wr_addr_i,
    input  logic [D_XLEN-1:0]          wr_data_i,
    output logic [D_XLEN-1:0]          rd_a_o,
    output logic [D_XLEN-1:0]          rd_b_o
);

  logic [D_XLEN-1:0] regs [DATA_REG_COUNT-1:0];
  integer            i;

  always_ff @(posedge clk_i or negedge rst_ni) begin
    if (!rst_ni) begin
      for (i = 0; i < DATA_REG_COUNT; i++) begin
        regs[i] <= '0;
      end
    end else begin
      if (wr_en_i && (wr_addr_i != REG_ZERO)) begin
        regs[wr_addr_i] <= wr_data_i;
      end
    end
  end

  always_comb begin
    rd_a_o = (req_i.ra == REG_ZERO) ? '0 : regs[req_i.ra];
    rd_b_o = (req_i.rb == REG_ZERO) ? '0 : regs[req_i.rb];
  end

endmodule

