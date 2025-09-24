`timescale 1ns/1ps

module amber128_dmem
  import amber128_pkg::*;
#(
    parameter string          INIT_FILE = "",
    parameter int unsigned    DEPTH     = 256   // number of 128-bit words
)(
    input  logic                  clk_i,
    input  logic                  rst_ni,
    input  logic                  req_i,
    input  logic                  we_i,
    input  logic [63:0]           addr_i,    // byte address, 16B aligned
    input  logic [C_XLEN-1:0]     wdata_i,
    output logic [C_XLEN-1:0]     rdata_o,
    output logic                  ready_o,
    output logic                  trap_o,
    output logic [7:0]            led_o,
    output logic                  uart_tx_valid_o,
    output logic [7:0]            uart_tx_data_o,
    input  logic                  uart_tx_ready_i
);

  localparam int unsigned ADDR_LSB    = $clog2(DMEM_WORD_BYTES); // 4
  localparam int unsigned INDEX_WIDTH = $clog2(DEPTH);
  localparam int unsigned HIGH_MSB    = ADDR_LSB + INDEX_WIDTH;

  logic [C_XLEN-1:0]           ram [0:DEPTH-1];
  logic [INDEX_WIDTH-1:0]      index;
  logic                        misaligned;
  logic                        out_of_bounds;
  logic [7:0]                  led_q;
  logic [7:0]                  uart_q;

  // Place MMIO registers at the top of the address space, for compatibility with prior prototypes
  localparam logic [INDEX_WIDTH-1:0] MMIO_LED_IDX  = INDEX_WIDTH'((DEPTH > 0) ? (DEPTH-1) : 0);
  localparam logic [INDEX_WIDTH-1:0] MMIO_UART_IDX = INDEX_WIDTH'((DEPTH > 1) ? (DEPTH-2) : ((DEPTH>0)?(DEPTH-1):0));
  logic is_mmio_led, is_mmio_uart;

  initial begin
    if (INIT_FILE != "") begin
      $readmemh(INIT_FILE, ram);
    end else begin : init_zero
      int i;
      for (i = 0; i < DEPTH; i++) begin
        ram[i] = '0;
      end
    end
  end

  assign index        = addr_i[ADDR_LSB +: INDEX_WIDTH];
  assign misaligned   = |addr_i[ADDR_LSB-1:0];
  assign out_of_bounds= (HIGH_MSB >= 64) ? 1'b0 : |addr_i[63:HIGH_MSB];
  assign is_mmio_led  = (index == MMIO_LED_IDX);
  assign is_mmio_uart = (index == MMIO_UART_IDX);

  always_ff @(posedge clk_i or negedge rst_ni) begin
    if (!rst_ni) begin
      ready_o         <= 1'b0;
      trap_o          <= 1'b0;
      rdata_o         <= '0;
      led_q           <= '0;
      uart_q          <= '0;
      led_o           <= '0;
      uart_tx_data_o  <= '0;
      uart_tx_valid_o <= 1'b0;
    end else begin
      ready_o         <= 1'b0;
      trap_o          <= 1'b0;
      uart_tx_valid_o <= 1'b0;

      if (req_i) begin
        if (misaligned || out_of_bounds) begin
          trap_o  <= 1'b1;
          rdata_o <= '0;
          ready_o <= 1'b1;
        end else if (is_mmio_uart) begin
          if (we_i) begin
            uart_q <= wdata_i[7:0];
            if (uart_tx_ready_i) begin
              uart_tx_data_o  <= wdata_i[7:0];
              uart_tx_valid_o <= 1'b1;
              rdata_o         <= {{(C_XLEN-8){1'b0}}, wdata_i[7:0]};
              ready_o         <= 1'b1;
            end
          end else begin
            rdata_o <= {{(C_XLEN-8){1'b0}}, uart_q};
            ready_o <= 1'b1;
          end
        end else begin
          logic [C_XLEN-1:0] mem_rdata;
          mem_rdata = ram[index];
          if (we_i) begin
            if (is_mmio_led) begin
              led_q <= wdata_i[7:0];
            end else begin
              ram[index] <= wdata_i;
              mem_rdata   = wdata_i;
            end
          end
          if (is_mmio_led) begin
            rdata_o <= {{(C_XLEN-8){1'b0}}, led_q};
          end else begin
            rdata_o <= mem_rdata;
          end
          ready_o <= 1'b1;
        end
      end

      led_o <= led_q;
    end
  end

endmodule

