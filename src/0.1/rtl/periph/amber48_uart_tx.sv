module amber48_uart_tx #(
    parameter int unsigned CLOCK_FREQ_HZ = 27_000_000,
    parameter int unsigned BAUD_RATE     = 115_200
)(
    input  logic       clk_i,
    input  logic       rst_ni,
    input  logic [7:0] data_i,
    input  logic       valid_i,
    output logic       ready_o,
    output logic       tx_o
);

  localparam int unsigned CLKS_PER_BIT = (CLOCK_FREQ_HZ + (BAUD_RATE/2)) / BAUD_RATE;
  localparam int unsigned CLK_CNT_WIDTH = (CLKS_PER_BIT <= 1) ? 1 : $clog2(CLKS_PER_BIT);
  // Pre-sized constants to avoid width truncation warnings in synthesis
  localparam logic [CLK_CNT_WIDTH-1:0] CLKS_PER_BIT_VEC = CLKS_PER_BIT[CLK_CNT_WIDTH-1:0];
  localparam logic [CLK_CNT_WIDTH-1:0] ONE_TICK         = {{(CLK_CNT_WIDTH-1){1'b0}}, 1'b1};
  localparam logic [CLK_CNT_WIDTH-1:0] CLKS_PER_BIT_M1  = CLKS_PER_BIT_VEC - ONE_TICK;

  initial begin
    if (CLKS_PER_BIT < 1) begin
      $fatal(1, "amber48_uart_tx: invalid CLOCK_FREQ_HZ/BAUD_RATE combination");
    end
  end

  typedef enum logic [1:0] {
    IDLE,
    START,
    DATA,
    STOP
  } state_e;

  state_e                   state_q;
  logic [CLK_CNT_WIDTH-1:0] clk_cnt_q;
  logic [2:0]               bit_idx_q;
  logic [7:0]               data_q;

  assign ready_o = (state_q == IDLE);

  always_ff @(posedge clk_i or negedge rst_ni) begin
    if (!rst_ni) begin
      state_q   <= IDLE;
      clk_cnt_q <= '0;
      bit_idx_q <= '0;
      data_q    <= '0;
      tx_o      <= 1'b1;
    end else begin
      case (state_q)
        IDLE: begin
          tx_o      <= 1'b1;
          clk_cnt_q <= CLKS_PER_BIT_M1;
          bit_idx_q <= '0;
          if (valid_i) begin
            data_q    <= data_i;
            state_q   <= START;
            tx_o      <= 1'b0;
          end
        end

        START: begin
          if (clk_cnt_q == 0) begin
            state_q   <= DATA;
            clk_cnt_q <= CLKS_PER_BIT_M1;
            tx_o      <= data_q[0];
            bit_idx_q <= '0;
          end else begin
            clk_cnt_q <= clk_cnt_q - ONE_TICK;
            tx_o      <= 1'b0;
          end
        end

        DATA: begin
          if (clk_cnt_q == 0) begin
            if (bit_idx_q == 3'd7) begin
              state_q   <= STOP;
              clk_cnt_q <= CLKS_PER_BIT_M1;
              tx_o      <= 1'b1;
            end else begin
              bit_idx_q <= bit_idx_q + 3'd1;
              clk_cnt_q <= CLKS_PER_BIT_M1;
              tx_o      <= data_q[bit_idx_q + 3'd1];
            end
          end else begin
            clk_cnt_q <= clk_cnt_q - ONE_TICK;
          end
        end

        STOP: begin
          if (clk_cnt_q == 0) begin
            state_q <= IDLE;
            tx_o    <= 1'b1;
          end else begin
            clk_cnt_q <= clk_cnt_q - ONE_TICK;
            tx_o      <= 1'b1;
          end
        end

        default: begin
          state_q <= IDLE;
          tx_o    <= 1'b1;
        end
      endcase
    end
  end

endmodule
