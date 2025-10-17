`timescale 1ns/1ps
`ifdef CPU_AD48_INSTR_VH
`undef CPU_AD48_INSTR_VH
`endif

module cpu_ad48_reset_tb;
  localparam IM_WORDS  = 64;
  localparam DM_WORDS  = 32;
  localparam IRQ_LINES = 4;

  reg clk;
  reg resetn;
  wire [IRQ_LINES-1:0] irq_lines = {IRQ_LINES{1'b0}};

  cpu_ad48 #(
    .IM_WORDS(IM_WORDS),
    .DM_WORDS(DM_WORDS),
    .IRQ_LINES(IRQ_LINES)
  ) dut (
    .clk   (clk),
    .resetn(resetn),
    .irq   (irq_lines)
  );

  `include "../../src/rtl/cpu_ad48_instr.vh"

  initial begin
    clk = 1'b0;
    forever #5 clk = ~clk;
  end

  task load_instr;
    input integer idx;
    input [47:0] value;
    begin
      dut.IMEM.mem[idx] = value;
    end
  endtask

  integer i;

  initial begin
    resetn = 1'b0;
    for (i = 0; i < IM_WORDS; i = i + 1) begin
      dut.IMEM.mem[i] = 48'd0;
    end

    // Program writes non-zero values into both register banks then halts.
    i = 0;
    load_instr(i++, instr_alui_a(1'b0, 3'd1, 3'd0, pack_subop(F_ADD), pack_imm27(to48(48'd42))));
    load_instr(i++, instr_alui_d(1'b1, 3'd1, 3'd0, pack_subop(F_ADD), pack_imm27(to48(48'h1234))));
    load_instr(i++, instr_sys(SYS_F_HALT));

    repeat (4) @(posedge clk);
    resetn = 1'b1;

    // Ensure program runs to completion and registers carry the programmed values.
    wait (dut.halt);
    @(posedge clk);

    if (dut.RF_A.regs[1] !== to48(48'd42)) begin
      $display("RESET TB: A1 setup failed. Got %h expected %h", dut.RF_A.regs[1], to48(48'd42));
      $fatal(1);
    end
    if (dut.RF_D.regs[1] !== to48(48'h1234)) begin
      $display("RESET TB: D1 setup failed. Got %h expected %h", dut.RF_D.regs[1], to48(48'h1234));
      $fatal(1);
    end

    // Apply asynchronous reset after state is non-zero.
    resetn = 1'b0;
    @(posedge clk);

    if (dut.RF_A.regs[1] !== 48'd0) begin
      $display("RESET TB: A1 did not clear during reset. value=%h", dut.RF_A.regs[1]);
      $fatal(1);
    end
    if (dut.RF_D.regs[1] !== 48'd0) begin
      $display("RESET TB: D1 did not clear during reset. value=%h", dut.RF_D.regs[1]);
      $fatal(1);
    end

    $display("cpu_ad48_reset_tb PASS");
    $finish;
  end
endmodule
