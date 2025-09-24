`timescale 1ns/1ps

module amber128_core_tb;
  import amber128_pkg::*;

  localparam string      IMEM_HEX   = "build/amber128_loop.hex";
  localparam int unsigned MAX_CYCLES = 200;

  logic clk;
  logic rst_ni;
  logic clk_en;
  logic rst_sync;

  logic [63:0]         imem_addr;
  logic [C_XLEN-1:0]   imem_data;
  logic                imem_valid;

  logic                trap;
  logic [2:0]          trap_cause;
  logic                retired;

  logic                dmem_req;
  logic                dmem_we;
  logic [63:0]         dmem_addr;
  logic [C_XLEN-1:0]   dmem_wdata;
  logic [C_XLEN-1:0]   dmem_rdata;
  logic                dmem_ready;
  logic                dmem_trap;

  /* verilator lint_off UNUSED */
  logic [7:0]          led_bus;
  logic                uart_tx_valid;
  logic [7:0]          uart_tx_data;
  /* verilator lint_on UNUSED */
  logic                uart_tx_ready;

  assign uart_tx_ready = 1'b1;

  amber128_core u_core (
      .clk_i        (clk),
      .rst_ni       (rst_ni),
      .clk_en_i     (clk_en),
      .imem_addr_o  (imem_addr),
      .imem_data_i  (imem_data),
      .imem_valid_i (imem_valid),
      .trap_o       (trap),
      .trap_cause_o (trap_cause),
      .retired_o    (retired),
      .dmem_req_o   (dmem_req),
      .dmem_we_o    (dmem_we),
      .dmem_addr_o  (dmem_addr),
      .dmem_wdata_o (dmem_wdata),
      .dmem_rdata_i (dmem_rdata),
      .dmem_ready_i (dmem_ready),
      .dmem_trap_i  (dmem_trap)
  );

  amber128_imem #(
      .INIT_FILE(IMEM_HEX)
  ) u_imem (
      .clk_i  (clk),
      .rst_ni (rst_ni),
      .addr_i (imem_addr),
      .data_o (imem_data),
      .valid_o(imem_valid)
  );

  amber128_dmem u_dmem (
      .clk_i          (clk),
      .rst_ni         (rst_ni),
      .req_i          (dmem_req),
      .we_i           (dmem_we),
      .addr_i         (dmem_addr),
      .wdata_i        (dmem_wdata),
      .rdata_o        (dmem_rdata),
      .ready_o        (dmem_ready),
      .trap_o         (dmem_trap),
      .led_o          (led_bus),
      .uart_tx_valid_o(uart_tx_valid),
      .uart_tx_data_o (uart_tx_data),
      .uart_tx_ready_i(uart_tx_ready)
  );

  integer cycle_count;
  integer retired_count;

  initial begin
    clk = 1'b0;
    forever #5 clk = ~clk;
  end

  initial begin
    rst_ni = 1'b0;
    clk_en = 1'b1;
    cycle_count = 0;
    retired_count = 0;
    $display("[TB] amber128 core loop test starting");
    repeat (5) @(posedge clk);
    rst_ni = 1'b1;
    $display("[TB] Deassert reset");
  end

  always_ff @(posedge clk or negedge rst_ni) begin
    if (!rst_ni) begin
      rst_sync <= 1'b0;
    end else begin
      rst_sync <= 1'b1;
    end
  end

  always @(posedge clk) begin
    if (!rst_sync) begin
      cycle_count   <= 0;
      retired_count <= 0;
    end else begin
      cycle_count <= cycle_count + 1;
      if (retired) begin
        retired_count <= retired_count + 1;
      end
      if (trap) begin
        $display("[TB][ERROR] Trap asserted at cycle %0d (cause=%0d)", cycle_count, trap_cause);
        $fatal(1, "Trap observed while running amber128 loop test");
      end
      if (cycle_count >= MAX_CYCLES) begin
        if (retired_count == 0) begin
          $fatal(1, "[TB][ERROR] No instructions retired within %0d cycles", MAX_CYCLES);
        end else begin
          $display("[TB][PASS] Completed %0d cycles with %0d retirements", cycle_count, retired_count);
          $finish;
        end
      end
    end
  end

endmodule
