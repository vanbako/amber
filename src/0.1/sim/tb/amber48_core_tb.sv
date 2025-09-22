`timescale 1ns/1ps

module amber48_core_tb;
  import amber48_pkg::*;

  logic clk;
  logic rst_ni;
  logic clk_en;

  logic [XLEN-1:0] imem_addr;
  logic [XLEN-1:0] imem_data;
  logic            imem_valid;

  logic            trap;
  logic [2:0]      trap_cause;
  logic            retired;

  logic            dmem_req;
  logic            dmem_we;
  logic [XLEN-1:0] dmem_addr;
  logic [XLEN-1:0] dmem_wdata;
  logic [XLEN-1:0] dmem_rdata;
  logic            dmem_ready;
  logic            dmem_trap;

  logic [7:0]      led_bus;
  logic            uart_tx_valid;
  logic [7:0]      uart_tx_data;

  amber48_core u_core (
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

  amber48_imem #(
      .INIT_FILE("build/amber48_smoke.hex")
  ) u_imem (
      .clk_i  (clk),
      .rst_ni (rst_ni),
      .addr_i (imem_addr),
      .data_o (imem_data),
      .valid_o(imem_valid)
  );

  amber48_dmem #(
      .INIT_FILE("")
  ) u_dmem (
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
      .uart_tx_data_o (uart_tx_data)
  );

  // Clock generation: 10 ns period
  initial clk = 1'b0;
  always #5 clk = ~clk;

  // Reset sequence
  initial begin
    rst_ni = 1'b0;
    clk_en = 1'b1;
    repeat (4) @(posedge clk);
    rst_ni = 1'b1;
  end

  // Simple UART monitor
  always @(posedge clk) begin
    if (uart_tx_valid) begin
      $display("[%0t] UART TX byte: 0x%0h (%c)", $time, uart_tx_data, uart_tx_data);
    end
  end

  // Finish simulation on trap or timeout
  initial begin
    int unsigned cycle_count = 0;
    wait (rst_ni);
    forever begin
      @(posedge clk);
      cycle_count++;
      if (trap) begin
        $display("[%0t] Trap asserted (cause=%0d), ending simulation", $time, trap_cause);
        #20 $finish;
      end
      if (cycle_count > 2000) begin
        $fatal(1, "Simulation timed out");
      end
    end
  end

  // Optional waveform dumping under SIM_TRACE
`ifdef SIM_TRACE
  initial begin
    $dumpfile("wave/amber48_core.fst");
    $dumpvars(0, amber48_core_tb);
  end
`endif

endmodule
