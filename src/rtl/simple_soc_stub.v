// Minimal wrapper to preload IMEM/DMEM from HEX files via $readmemh
module simple_soc_stub #(
  parameter IM_WORDS = 16384,
  parameter DM_WORDS = 16384,
  parameter IRQ_LINES = 4
)(
  input clk,
  input resetn,
  input [IRQ_LINES-1:0] irq_lines
);
  // Instantiate CPU with exposed memories if you prefer direct preload,
  // otherwise keep the CPU's internal memories and modify mem48 to $readmemh.
  cpu_ad48 #(
    .IM_WORDS(IM_WORDS),
    .DM_WORDS(DM_WORDS),
    .IRQ_LINES(IRQ_LINES)
  ) CPU (
    .clk   (clk),
    .resetn(resetn),
    .irq   (irq_lines)
  );
endmodule
