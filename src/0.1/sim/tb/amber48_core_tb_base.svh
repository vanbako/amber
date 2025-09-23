`ifndef TB_DMEM_FILE
  `define TB_DMEM_FILE ""
`endif
`ifndef TB_MAX_CYCLES
  `define TB_MAX_CYCLES 5000000
`endif
module `TB_MODULE_NAME;
  import amber48_pkg::*;

  localparam string          TEST_NAME        = `TB_NAME;
  localparam amber48_trap_e  EXPECTED_TRAP_VAL= `TB_EXPECTED_TRAP;
  localparam integer         MAX_CYCLES_VAL   = `TB_MAX_CYCLES;

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
  logic            uart_tx_ready;
  logic            uart_tx;

  logic            trap_seen;
  integer          cycle_count;
  integer          uart_index;
  integer          uart_count;
  amber48_trap_e   actual_trap;
  amber48_trap_e   expected_trap_e;
  logic            store0_seen;

  function string trap_to_string(amber48_trap_e trap);
    case (trap)
      TRAP_NONE:       trap_to_string = "TRAP_NONE";
      TRAP_ILLEGAL:    trap_to_string = "TRAP_ILLEGAL";
      TRAP_DATA_FAULT: trap_to_string = "TRAP_DATA_FAULT";
      default:         trap_to_string = $sformatf("UNKNOWN_%0d", trap);
    endcase
  endfunction

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
      .INIT_FILE(`TB_IMEM_FILE)
  ) u_imem (
      .clk_i   (clk),
      .rst_ni  (rst_ni),
      .addr_i  (imem_addr),
      .data_o  (imem_data),
      .valid_o (imem_valid)
  );

  amber48_dmem #(
      .INIT_FILE(`TB_DMEM_FILE)
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
      .uart_tx_data_o (uart_tx_data),
      .uart_tx_ready_i(uart_tx_ready),
      .aux_en_i       (1'b0),
      .aux_addr_i     ('0),
      .aux_rdata_o    ()
  );

  amber48_uart_tx #(
      .CLOCK_FREQ_HZ(100_000_000),
      .BAUD_RATE    (1_000_000)
  ) u_uart_tx (
      .clk_i  (clk),
      .rst_ni (rst_ni),
      .data_i (uart_tx_data),
      .valid_i(uart_tx_valid),
      .ready_o(uart_tx_ready),
      .tx_o   (uart_tx)
  );

  initial clk = 1'b0;
  always #5 clk = ~clk;

  initial begin
    rst_ni = 1'b0;
    clk_en = 1'b1;
    trap_seen = 1'b0;
    cycle_count = 0;
    uart_index = 0;
    uart_count = 0;
    store0_seen = 1'b0;
    repeat (4) @(posedge clk);
    rst_ni = 1'b1;
  end

  always @(posedge clk) begin
    if (!rst_ni) begin
      uart_index <= 0;
      uart_count <= 0;
    end else if (uart_tx_valid && uart_tx_ready) begin
      uart_index <= uart_index + 1;
      uart_count <= uart_count + 1;
      if (uart_tx_data >= 8'h20 && uart_tx_data <= 8'h7E) begin
        $display("[%0t][%s] UART TX byte: 0x%0h (%c)", $time, TEST_NAME, uart_tx_data, uart_tx_data);
      end else begin
        $display("[%0t][%s] UART TX byte: 0x%0h", $time, TEST_NAME, uart_tx_data);
      end
    end
  end

  always @(posedge clk) begin
    if (!rst_ni) begin
      store0_seen <= 1'b0;
    end else if (dmem_req && dmem_ready && dmem_we && (dmem_addr == '0)) begin
      store0_seen <= 1'b1;
    end
  end

  always @(posedge clk) begin
    if (!rst_ni) begin
      trap_seen <= 1'b0;
    end else if (trap && !trap_seen) begin
      trap_seen <= 1'b1;
      actual_trap    = amber48_trap_e'(trap_cause);
      expected_trap_e = EXPECTED_TRAP_VAL;
      if (actual_trap !== expected_trap_e) begin
        $fatal(1, "[%0t][%s] Unexpected trap cause %s (expected %s)",
               $time, TEST_NAME, trap_to_string(actual_trap), trap_to_string(expected_trap_e));
      end
      if (`TB_EXPECT_STORE_ADDR0) begin
        if (!store0_seen) begin
          $fatal(1, "[%0t][%s] Expected DMEM[0] store not observed", $time, TEST_NAME);
        end else begin
          $display("[%0t][%s] Observed DMEM[0] store", $time, TEST_NAME);
        end
      end else if (store0_seen) begin
        $fatal(1, "[%0t][%s] Unexpected DMEM[0] store", $time, TEST_NAME);
      end
      if (uart_count != 0) begin
        $display("[%0t][%s] UART produced %0d bytes", $time, TEST_NAME, uart_count);
      end
      $display("[%0t][%s] Trap asserted (cause=%s), ending simulation",
               $time, TEST_NAME, trap_to_string(actual_trap));
      repeat (5) @(posedge clk);
      $finish;
    end
  end

  initial begin
    wait (rst_ni);
    forever begin
      @(posedge clk);
      cycle_count = cycle_count + 1;
      if (cycle_count > MAX_CYCLES_VAL) begin
        $fatal(1, "[%0t][%s] Simulation timed out after %0d cycles",
               $time, TEST_NAME, cycle_count);
      end
    end
  end

`ifdef SIM_TRACE
  initial begin
    $dumpfile("wave/amber48_core.fst");
    $dumpvars(0, `TB_MODULE_NAME);
  end
`endif

endmodule

`undef TB_MODULE_NAME
`undef TB_NAME
`undef TB_IMEM_FILE
`undef TB_DMEM_FILE
`undef TB_EXPECTED_TRAP
`undef TB_MAX_CYCLES
`undef TB_EXPECTED_UART
`undef TB_EXPECT_STORE_ADDR0
