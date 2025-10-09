`timescale 1ns/1ps
`ifdef CPU_AD48_INSTR_VH
`undef CPU_AD48_INSTR_VH
`endif

module cpu_ad48_alu_tb;
  localparam IM_WORDS = 64;
  localparam DM_WORDS = 64;

  reg clk;
  reg resetn;

  cpu_ad48 #(
    .IM_WORDS(IM_WORDS),
    .DM_WORDS(DM_WORDS)
  ) dut (
    .clk   (clk),
    .resetn(resetn)
  );

  `include "../../../src/0.1/rtl/cpu_ad48_instr.vh"

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

  task expect_a;
    input [2:0] idx;
    input [47:0] value;
    begin
      if (dut.RF_A.regs[idx] !== value) begin
        $display("ALU TB: A%0d mismatch. Got %h expected %h", idx, dut.RF_A.regs[idx], value);
        $fatal(1);
      end
    end
  endtask

  task expect_d;
    input [2:0] idx;
    input [47:0] value;
    begin
      if (dut.RF_D.regs[idx] !== value) begin
        $display("ALU TB: D%0d mismatch. Got %h expected %h", idx, dut.RF_D.regs[idx], value);
        $fatal(1);
      end
    end
  endtask

  integer i;
  initial begin
    resetn = 1'b0;

    // Clear memories
    for (i = 0; i < IM_WORDS; i = i + 1) dut.IMEM.mem[i] = 48'd0;
    for (i = 0; i < DM_WORDS; i = i + 1) dut.DMEM.mem[i] = 48'd0;

    // Program to exercise ALU paths and swap logic
    load_instr( 0, instr_alu   (1'b1, 3'd0, 3'd0, 3'd0, F_NOT, 1'b0)); // D0 = ~0
    load_instr( 1, instr_alui_d(1'b1, 3'd0, 3'd0, pack_subop(F_ADD), pack_imm27(48'd1))); // D0 = 0
    load_instr( 2, instr_alui_a(1'b0, 3'd1, 3'd0, pack_subop(F_ADD), pack_imm27(48'd5))); // A1 = 5
    load_instr( 3, instr_alui_d(1'b1, 3'd1, 3'd0, pack_subop(F_ADD), pack_imm27(48'd7))); // D1 = 7
    load_instr( 4, instr_alu   (1'b1, 3'd2, 3'd1, 3'd1, F_ADD, 1'b0)); // D2 = 12
    load_instr( 5, instr_alu   (1'b0, 3'd2, 3'd1, 3'd1, F_SUB, 1'b0)); // A2 = -2
    load_instr( 6, instr_alu   (1'b1, 3'd3, 3'd1, 3'd1, F_AND, 1'b0)); // D3 = 5
    load_instr( 7, instr_alu   (1'b1, 3'd4, 3'd1, 3'd1, F_OR , 1'b0)); // D4 = 7
    load_instr( 8, instr_alu   (1'b1, 3'd5, 3'd1, 3'd1, F_XOR, 1'b0)); // D5 = 2
    load_instr( 9, instr_alui_a(1'b0, 3'd3, 3'd1, pack_subop(F_SLL), pack_imm27(48'd1))); // A3 = 10
    load_instr(10, instr_alui_d(1'b1, 3'd6, 3'd1, pack_subop(F_SRL), pack_imm27(48'd1))); // D6 = 3
    load_instr(11, instr_alui_d(1'b1, 3'd7, 3'd1, pack_subop(F_SRA), pack_imm27(48'd1))); // D7 = 3
    load_instr(12, instr_alui_a(1'b0, 3'd4, 3'd1, pack_subop(F_NOT), pack_imm27(48'd0))); // A4 = ~5
    load_instr(13, instr_alui_a(1'b0, 3'd0, 3'd1, pack_subop(F_ADD), pack_imm27(48'd123))); // Attempt write to A0 (ignored)
    load_instr(14, instr_alu   (1'b1, 3'd0, 3'd0, 3'd1, F_ADD, 1'b0)); // D0 = A0 + D1 = 7
    load_instr(15, instr_alu   (1'b1, 3'd2, 3'd3, 3'd1, F_ADD, 1'b1)); // D2 = D1 + A3 = 17 (swap)
    load_instr(16, instr_alui_a(1'b0, 3'd5, 3'd3, pack_subop(F_ADD), pack_imm27(to48(-8)))); // A5 = 2
    load_instr(17, instr_alu   (1'b1, 3'd7, 3'd0, 3'd1, F_SUB, 1'b0)); // D7 = -7
    load_instr(18, instr_alui_d(1'b1, 3'd7, 3'd7, pack_subop(F_SRA), pack_imm27(48'd1))); // D7 = -4 (arith shift)
    load_instr(19, instr_sys(4'hF));

    // Hold reset for a couple of cycles
    repeat (4) @(posedge clk);
    resetn = 1'b1;

    // Wait for HALT
    wait (dut.halt);
    @(posedge clk);

    expect_a(3'd1, to48(5));
    expect_a(3'd2, to48(-2));
    expect_a(3'd3, to48(10));
    expect_a(3'd4, to48(~48'd5));
    expect_a(3'd5, to48(2));

    expect_d(3'd0, to48(7));
    expect_d(3'd1, to48(7));
    expect_d(3'd2, to48(17));
    expect_d(3'd3, to48(5));
    expect_d(3'd4, to48(7));
    expect_d(3'd5, to48(2));
    expect_d(3'd6, to48(3));
    expect_d(3'd7, to48(-4));

    $display("cpu_ad48_alu_tb PASS");
    $finish;
  end
endmodule
