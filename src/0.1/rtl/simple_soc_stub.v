// Minimal wrapper to preload IMEM/DMEM from HEX files via $readmemh
module simple_soc_stub(
  input clk, input resetn
);
  // Instantiate CPU with exposed memories if you prefer direct preload,
  // otherwise keep the CPU's internal memories and modify mem48 to $readmemh.
  cpu_ad48 #(.IM_WORDS(16384), .DM_WORDS(16384)) CPU (
    .clk(clk), .resetn(resetn)
  );
endmodule
