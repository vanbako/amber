`timescale 1ns/1ps

// Higher-level SoC simulation wrapper that exercises the AD48 core through the
// simple SoC stub. Mirrors the run_core_tb interface so existing manifests can
// be reused with a different --sim-top selection.
module run_soc_tb;
  parameter integer IM_WORDS = 16384;
  parameter integer DM_WORDS = 16384;
  parameter integer CLK_PERIOD_NS = 10;
  parameter integer IRQ_LINES = 4;

  reg clk = 1'b0;
  reg resetn = 1'b0;
  reg [IRQ_LINES-1:0] irq_lines = {IRQ_LINES{1'b0}};

  integer cycle_count = 0;
  integer max_cycles = 0;

  reg [1023:0] imem_path;
  reg [1023:0] dmem_path;
  reg [1023:0] vcd_path;

  simple_soc_stub #(
    .IM_WORDS(IM_WORDS),
    .DM_WORDS(DM_WORDS),
    .IRQ_LINES(IRQ_LINES)
  ) dut (
    .clk(clk),
    .resetn(resetn),
    .irq_lines(irq_lines)
  );

  initial begin
    forever #(CLK_PERIOD_NS / 2) clk = ~clk;
  end

  initial begin
    resetn = 1'b0;
    repeat (4) @(posedge clk);
    resetn = 1'b1;
  end

  initial begin
    if (!$value$plusargs("IMEM=%s", imem_path)) begin
      $display("[run_soc_tb] ERROR: +IMEM=<path> plusarg is required");
      $fatal(1);
    end
    $display("[run_soc_tb] Loading IMEM from %0s", imem_path);
    $readmemh(imem_path, dut.CPU.IMEM.mem);

    if ($value$plusargs("DMEM=%s", dmem_path)) begin
      $display("[run_soc_tb] Loading DMEM from %0s", dmem_path);
      $readmemh(dmem_path, dut.CPU.DMEM.mem);
    end

    if ($value$plusargs("MAX_CYCLES=%d", max_cycles)) begin
      if (max_cycles < 0) max_cycles = 0;
      if (max_cycles > 0)
        $display("[run_soc_tb] Cycle limit set to %0d", max_cycles);
    end

    if ($value$plusargs("VCD=%s", vcd_path)) begin
      $display("[run_soc_tb] Writing VCD to %0s", vcd_path);
      $dumpfile(vcd_path);
      $dumpvars(0, run_soc_tb);
    end
  end

  always @(posedge clk) begin
    if (!resetn) begin
      cycle_count <= 0;
    end else begin
      cycle_count <= cycle_count + 1;
      if (max_cycles > 0 && cycle_count >= max_cycles) begin
        $display("[run_soc_tb] ERROR: timeout after %0d cycles", cycle_count);
        $fatal(1);
      end
    end
  end

  always @(posedge clk) begin
    if (resetn && dut.CPU.halt) begin
      $display("[run_soc_tb] HALT after %0d cycles (PC=%0d)", cycle_count, dut.CPU.pc);
      $finish;
    end
  end
endmodule
