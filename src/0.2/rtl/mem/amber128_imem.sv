`timescale 1ns/1ps

module amber128_imem
  import amber128_pkg::*;
#(
    parameter string          INIT_FILE = "",
    parameter int unsigned    DEPTH     = 256   // number of 128-bit words
)(
    input  logic                  clk_i,
    input  logic                  rst_ni,
    input  logic [63:0]           addr_i,   // byte address, must be 16B aligned
    output logic [C_XLEN-1:0]     data_o,
    output logic                  valid_o
);

  localparam int unsigned ADDR_LSB    = $clog2(IMEM_WORD_BYTES); // 4
  localparam int unsigned INDEX_WIDTH = $clog2(DEPTH);

  logic [C_XLEN-1:0]           rom [0:DEPTH-1];
  logic [INDEX_WIDTH-1:0]      index;
  logic [C_XLEN-1:0]           data_q;

  initial begin
    if (INIT_FILE != "") begin
      $readmemh(INIT_FILE, rom);
    end else begin : init_zero
      int i;
      for (i = 0; i < DEPTH; i++) begin
        rom[i] = '0;
      end
    end
  end

  if (ADDR_LSB > 0) begin : gen_unused_addr_low
    logic unused_addr_lo;
    assign unused_addr_lo = |addr_i[ADDR_LSB-1:0];
  end

  if ((ADDR_LSB + INDEX_WIDTH) < 64) begin : gen_unused_addr_bits
    logic unused_addr_hi;
    assign unused_addr_hi = |addr_i[63:ADDR_LSB + INDEX_WIDTH];
  end

  assign index = addr_i[ADDR_LSB +: INDEX_WIDTH];

  always_ff @(posedge clk_i or negedge rst_ni) begin
    if (!rst_ni) begin
      data_q  <= '0;
      valid_o <= 1'b0;
    end else begin
      data_q  <= rom[index];
      valid_o <= 1'b1;
    end
  end

  assign data_o = data_q;

endmodule

