module amber48_top
  import amber48_pkg::*;
(
    input  logic clk_27mhz_i,
    input  logic rst_button_ni,
    output logic led0_o,
    output logic [7:0] led_mmio_o,
    output logic       uart_tx_valid_o,
    output logic [7:0] uart_tx_data_o
);

  logic rst_sync_n;
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
  logic            heartbeat_q;

  reset_sync u_reset_sync (
      .clk_i (clk_27mhz_i),
      .rst_ni(rst_button_ni),
      .rst_no(rst_sync_n)
  );

  amber48_core u_core (
      .clk_i        (clk_27mhz_i),
      .rst_ni       (rst_sync_n),
      .clk_en_i     (1'b1),
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
      .clk_i  (clk_27mhz_i),
      .rst_ni (rst_sync_n),
      .addr_i (imem_addr),
      .data_o (imem_data),
      .valid_o(imem_valid)
  );

  amber48_dmem #(
      .INIT_FILE("")
  ) u_dmem (
      .clk_i          (clk_27mhz_i),
      .rst_ni         (rst_sync_n),
      .req_i          (dmem_req),
      .we_i           (dmem_we),
      .addr_i         (dmem_addr),
      .wdata_i        (dmem_wdata),
      .rdata_o        (dmem_rdata),
      .ready_o        (dmem_ready),
      .trap_o         (dmem_trap),
      .led_o          (led_bus),
      .uart_tx_valid_o(uart_tx_valid_o),
      .uart_tx_data_o (uart_tx_data_o)
  );

  // Simple heartbeat toggles each retired instruction.
  always_ff @(posedge clk_27mhz_i or negedge rst_sync_n) begin
    if (!rst_sync_n) begin
      heartbeat_q <= 1'b0;
    end else if (retired) begin
      heartbeat_q <= ~heartbeat_q;
    end
  end

  assign led0_o      = heartbeat_q;
  assign led_mmio_o  = led_bus;

endmodule
