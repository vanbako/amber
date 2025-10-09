`timescale 1ns/1ps
`ifdef CPU_AD48_INSTR_VH
`undef CPU_AD48_INSTR_VH
`endif

module cpu_ad48_csr_tb;
  localparam IM_WORDS = 128;
  localparam DM_WORDS = 32;
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
  localparam [11:0] CSR_ADDR_INVALID = 12'h3FF;

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

  task expect_d;
    input [2:0] idx;
    input [47:0] value;
    begin
      if (dut.RF_D.regs[idx] !== value) begin
        $display("CSR TB: D%0d mismatch. Got %h expected %h", idx, dut.RF_D.regs[idx], value);
        $fatal(1);
      end
    end
  endtask

  integer pc;

  initial begin
    resetn = 1'b0;

    for (pc = 0; pc < IM_WORDS; pc = pc + 1) begin
      dut.IMEM.mem[pc] = 48'd0;
    end
    pc = 0;

    // 0: Read initial status into D1.
    load_instr(pc++, instr_csr(CSR_F_R, 1'b1, 3'd1, 3'd0, pack_csr_addr(CSR_ADDR_STATUS)));
    // 1: D2 = 0x12340.
    load_instr(pc++, instr_alui_d(1'b1, 3'd2, 3'd2, pack_subop(F_ADD), pack_imm27(to48(48'h12340))));
    // 2: Write scratch <- D2 (discard readback).
    load_instr(pc++, instr_csr(CSR_F_RW, 1'b0, 3'd0, 3'd2, pack_csr_addr(CSR_ADDR_SCRATCH)));
    // 3: Read scratch into D3.
    load_instr(pc++, instr_csr(CSR_F_R, 1'b1, 3'd3, 3'd0, pack_csr_addr(CSR_ADDR_SCRATCH)));
    // 4: D4 = 0x000F mask.
    load_instr(pc++, instr_alui_d(1'b1, 3'd4, 3'd4, pack_subop(F_ADD), pack_imm27(to48(48'h000F))));
    // 5: CSR set bits (old value returned in D5).
    load_instr(pc++, instr_csr(CSR_F_RS, 1'b1, 3'd5, 3'd4, pack_csr_addr(CSR_ADDR_SCRATCH)));
    // 6: Read scratch into D6 (should include mask bits).
    load_instr(pc++, instr_csr(CSR_F_R, 1'b1, 3'd6, 3'd0, pack_csr_addr(CSR_ADDR_SCRATCH)));
    // 7: CSR clear bits (old value returned in D7).
    load_instr(pc++, instr_csr(CSR_F_RC, 1'b1, 3'd7, 3'd4, pack_csr_addr(CSR_ADDR_SCRATCH)));
    // 8: Read scratch into D2 (should have mask cleared).
    load_instr(pc++, instr_csr(CSR_F_R, 1'b1, 3'd2, 3'd0, pack_csr_addr(CSR_ADDR_SCRATCH)));
    // 9: Read cycle counter into D3.
    load_instr(pc++, instr_csr(CSR_F_R, 1'b1, 3'd3, 3'd0, pack_csr_addr(CSR_ADDR_CYCLE)));
    // 10: Read cycle counter again into D4.
    load_instr(pc++, instr_csr(CSR_F_R, 1'b1, 3'd4, 3'd0, pack_csr_addr(CSR_ADDR_CYCLE)));
    // 11: D0 = 0x123003 for status write (privilege bits set to 3).
    load_instr(pc++, instr_alui_d(1'b1, 3'd0, 3'd0, pack_subop(F_ADD), pack_imm27(to48(48'h123003))));
    // 12: Write status <- D0 (discard readback).
    load_instr(pc++, instr_csr(CSR_F_RW, 1'b0, 3'd0, 3'd0, pack_csr_addr(CSR_ADDR_STATUS)));
    // 13: Read status into D6.
    load_instr(pc++, instr_csr(CSR_F_R, 1'b1, 3'd6, 3'd0, pack_csr_addr(CSR_ADDR_STATUS)));
    // 14: Attempt to read invalid CSR into D7 (should be ignored).
    load_instr(pc++, instr_csr(CSR_F_R, 1'b1, 3'd7, 3'd0, pack_csr_addr(CSR_ADDR_INVALID)));
    // 15: HALT.
    load_instr(pc++, instr_sys(4'hF));

    repeat (4) @(posedge clk);
    resetn = 1'b1;

    wait (dut.halt);
    @(posedge clk);

    // General register checks.
    expect_d(3'd1, to48(48'h3)); // status at reset -> machine mode
    expect_d(3'd2, to48(48'h12340)); // scratch restored after set/clear
    expect_d(3'd5, to48(48'h12340)); // csr.rs returned original scratch
    expect_d(3'd6, to48(48'h123003)); // status readback after write
    expect_d(3'd7, to48(48'h1234F)); // csr.rc old value preserved (invalid read ignored)

    if (!(dut.RF_D.regs[4] > dut.RF_D.regs[3])) begin
      $display("CSR TB: cycle readings not increasing. before=%h after=%h",
               dut.RF_D.regs[3], dut.RF_D.regs[4]);
      $fatal(1);
    end

    if (dut.csr_scratch !== to48(48'h12340)) begin
      $display("CSR TB: scratch CSR mismatch. Got %h expected %h", dut.csr_scratch, to48(48'h12340));
      $fatal(1);
    end

    if (dut.csr_status !== to48(48'h123003)) begin
      $display("CSR TB: status CSR mismatch. Got %h expected %h", dut.csr_status, to48(48'h123003));
      $fatal(1);
    end

    if (dut.priv_mode !== 2'b11) begin
      $display("CSR TB: priv_mode mismatch. Got %b expected 11", dut.priv_mode);
      $fatal(1);
    end

    if (dut.csr_instret !== to48(48'd15)) begin
      $display("CSR TB: instret mismatch. Got %0d expected %0d", dut.csr_instret, 15);
      $fatal(1);
    end

    if (dut.csr_cycle < dut.csr_instret) begin
      $display("CSR TB: cycle counter less than instret. cycle=%0d instret=%0d",
               dut.csr_cycle, dut.csr_instret);
      $fatal(1);
    end

    $display("cpu_ad48_csr_tb PASS");
    $finish;
  end
endmodule
