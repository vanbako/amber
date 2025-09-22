module amber48_dmem
  import amber48_pkg::*;
#(
    parameter string              INIT_FILE = "",
    parameter int unsigned        DEPTH     = DMEM_DEPTH
)(
    input  logic                  clk_i,
    input  logic                  rst_ni,
    input  logic                  req_i,
    input  logic                  we_i,
    input  logic [XLEN-1:0]       addr_i,
    input  logic [XLEN-1:0]       wdata_i,
    output logic [XLEN-1:0]       rdata_o,
    output logic                  ready_o,
    output logic                  trap_o,
    output logic [7:0]            led_o,
    output logic                  uart_tx_valid_o,
    output logic [7:0]            uart_tx_data_o
);

  localparam int unsigned ADDR_LSB     = $clog2(BAU_BYTES);
  localparam int unsigned INDEX_WIDTH  = $clog2(DEPTH);
  localparam int unsigned HIGH_MSB     = ADDR_LSB + INDEX_WIDTH;
  localparam int unsigned MMIO_LED_IDX = DEPTH > 0 ? DEPTH-1 : 0;
  localparam int unsigned MMIO_UART_IDX= DEPTH > 1 ? DEPTH-2 : MMIO_LED_IDX;

  logic [XLEN-1:0]              ram [0:DEPTH-1];
  logic [INDEX_WIDTH-1:0]       addr_index;
  logic                         misaligned;
  logic                         out_of_bounds;
  logic                         is_mmio_led;
  logic                         is_mmio_uart;
  logic [7:0]                   led_q;
  logic [7:0]                   uart_q;

  initial begin
    if (DEPTH < 2) begin
      $fatal(1, "amber48_dmem: DEPTH must be >= 2");
    end
    if (INIT_FILE != "") begin
      $readmemh(INIT_FILE, ram);
    end else begin : init_zero
      integer i;
      for (i = 0; i < DEPTH; i++) begin
        ram[i] = '0;
      end
    end
  end

  assign addr_index = addr_i[ADDR_LSB +: INDEX_WIDTH];
  assign misaligned = (ADDR_LSB == 0) ? 1'b0 : |addr_i[ADDR_LSB-1:0];
  assign out_of_bounds = (HIGH_MSB >= XLEN) ? 1'b0 : |addr_i[XLEN-1:HIGH_MSB];
  assign is_mmio_led  = (addr_index == MMIO_LED_IDX);
  assign is_mmio_uart = (addr_index == MMIO_UART_IDX);

  always_ff @(posedge clk_i or negedge rst_ni) begin
    if (!rst_ni) begin
      ready_o          <= 1'b0;
      trap_o           <= 1'b0;
      rdata_o          <= '0;
      led_q            <= '0;
      uart_q           <= '0;
      led_o            <= '0;
      uart_tx_data_o   <= '0;
      uart_tx_valid_o  <= 1'b0;
    end else begin
      ready_o          <= 1'b0;
      trap_o           <= 1'b0;
      uart_tx_valid_o  <= 1'b0;

      if (req_i) begin
        if (misaligned || out_of_bounds) begin
          trap_o <= 1'b1;
          rdata_o <= '0;
          ready_o <= 1'b1;
        end else begin
          logic [XLEN-1:0] mem_rdata;
          mem_rdata = ram[addr_index];

          if (we_i) begin
            if (is_mmio_led) begin
              led_q <= wdata_i[7:0];
            end else if (is_mmio_uart) begin
              uart_q          <= wdata_i[7:0];
              uart_tx_data_o  <= wdata_i[7:0];
              uart_tx_valid_o <= 1'b1;
            end else begin
              ram[addr_index] <= wdata_i;
              mem_rdata        = wdata_i;
            end
          end

          if (is_mmio_led) begin
            rdata_o <= {{(XLEN-8){1'b0}}, led_q};
          end else if (is_mmio_uart) begin
            rdata_o <= {{(XLEN-8){1'b0}}, uart_q};
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
