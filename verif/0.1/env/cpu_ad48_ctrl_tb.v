`timescale 1ns/1ps
`include "cpu_ad48_instr.vh"

module cpu_ad48_ctrl_tb;
  localparam IM_WORDS = 128;
  localparam DM_WORDS = 32;

  reg clk;
  reg resetn;

  cpu_ad48 #(
    .IM_WORDS(IM_WORDS),
    .DM_WORDS(DM_WORDS)
  ) dut (
    .clk   (clk),
    .resetn(resetn)
  );

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
        $display("CTRL TB: A%0d mismatch. Got %h expected %h", idx, dut.RF_A.regs[idx], value);
        $fatal(1);
      end
    end
  endtask

  task expect_d;
    input [2:0] idx;
    input [47:0] value;
    begin
      if (dut.RF_D.regs[idx] !== value) begin
        $display("CTRL TB: D%0d mismatch. Got %h expected %h", idx, dut.RF_D.regs[idx], value);
        $fatal(1);
      end
    end
  endtask

  integer pc;
  integer idx_beq, idx_bne, idx_blt, idx_bltu, idx_bge, idx_bgeu;
  integer idx_loop_branch, idx_jal_call, idx_jal_skip;
  integer loop_start;
  integer off;
  integer sub_idx;

  initial begin
    resetn = 1'b0;

    for (pc = 0; pc < IM_WORDS; pc = pc + 1) begin
      dut.IMEM.mem[pc] = 48'd0;
    end
    pc = 0;

    // Setup operands
    load_instr(pc++, instr_alui_a(1'b0, 3'd1, 3'd1, pack_subop(F_ADD), pack_imm27(to48(5))));   // A1 = 5
    load_instr(pc++, instr_alui_d(1'b1, 3'd1, 3'd1, pack_subop(F_ADD), pack_imm27(to48(5))));   // D1 = 5
    load_instr(pc++, instr_alui_a(1'b0, 3'd2, 3'd2, pack_subop(F_ADD), pack_imm27(to48(-7))));  // A2 = -7
    load_instr(pc++, instr_alui_d(1'b1, 3'd2, 3'd2, pack_subop(F_ADD), pack_imm27(to48(-3))));  // D2 = -3
    load_instr(pc++, instr_alui_a(1'b0, 3'd3, 3'd3, pack_subop(F_ADD), pack_imm27(to48(8))));   // A3 = 8
    load_instr(pc++, instr_alui_d(1'b1, 3'd3, 3'd3, pack_subop(F_ADD), pack_imm27(to48(9))));   // D3 = 9
    load_instr(pc++, instr_alui_d(1'b1, 3'd4, 3'd4, pack_subop(F_ADD), pack_imm27(to48(1))));   // D4 = 1
    load_instr(pc++, instr_alui_d(1'b1, 3'd5, 3'd5, pack_subop(F_ADD), pack_imm27(to48(0))));   // D5 = 0
    load_instr(pc++, instr_alui_d(1'b1, 3'd6, 3'd6, pack_subop(F_ADD), pack_imm27(to48(0))));   // D6 = 0
    load_instr(pc++, instr_alui_d(1'b1, 3'd7, 3'd7, pack_subop(F_ADD), pack_imm27(to48(0))));   // D7 = 0
    load_instr(pc++, instr_alui_a(1'b0, 3'd4, 3'd4, pack_subop(F_ADD), pack_imm27(to48(-1))));  // A4 = -1
    load_instr(pc++, instr_alui_a(1'b0, 3'd5, 3'd5, pack_subop(F_ADD), pack_imm27(to48(2))));   // A5 = 2
    load_instr(pc++, instr_alui_d(1'b1, 3'd0, 3'd0, pack_subop(F_ADD), pack_imm27(to48(0))));   // D0 = 0

    idx_beq = pc; load_instr(pc++, 48'd0);                                                      // placeholder
    load_instr(pc++, instr_alui_d(1'b1, 3'd4, 3'd4, pack_subop(F_ADD), pack_imm27(to48(1))));   // would inc D4
    idx_bne = pc; load_instr(pc++, 48'd0);
    load_instr(pc++, instr_alui_d(1'b1, 3'd5, 3'd5, pack_subop(F_ADD), pack_imm27(to48(1))));   // D5 += 1 (should execute)
    idx_blt = pc; load_instr(pc++, 48'd0);
    load_instr(pc++, instr_alui_d(1'b1, 3'd6, 3'd6, pack_subop(F_ADD), pack_imm27(to48(1))));   // D6 += 1 (skip)
    idx_bltu = pc; load_instr(pc++, 48'd0);
    load_instr(pc++, instr_alui_d(1'b1, 3'd7, 3'd7, pack_subop(F_ADD), pack_imm27(to48(1))));   // D7 += 1 (execute)
    idx_bge = pc; load_instr(pc++, 48'd0);
    load_instr(pc++, instr_alui_d(1'b1, 3'd5, 3'd5, pack_subop(F_ADD), pack_imm27(to48(1))));   // D5 += 1 (skip)
    idx_bgeu = pc; load_instr(pc++, 48'd0);
    load_instr(pc++, instr_alui_d(1'b1, 3'd6, 3'd6, pack_subop(F_ADD), pack_imm27(to48(1))));   // D6 += 1 (skip)

    loop_start = pc;
    load_instr(pc++, instr_alui_a(1'b0, 3'd5, 3'd5, pack_subop(F_ADD), pack_imm27(to48(-1))));  // A5 -= 1
    load_instr(pc++, instr_alui_d(1'b1, 3'd7, 3'd7, pack_subop(F_ADD), pack_imm27(to48(1))));   // D7 += 1
    idx_loop_branch = pc; load_instr(pc++, 48'd0);                                              // loop branch

    idx_jal_call = pc; load_instr(pc++, 48'd0);                                                 // JAL call placeholder
    load_instr(pc++, instr_alui_d(1'b1, 3'd2, 3'd2, pack_subop(F_ADD), pack_imm27(to48(42))));  // D2 += 42 after return
    idx_jal_skip = pc; load_instr(pc++, 48'd0);                                                 // JAL skip placeholder
    load_instr(pc++, instr_alui_d(1'b1, 3'd6, 3'd6, pack_subop(F_ADD), pack_imm27(to48(5))));   // D6 += 5 (should skip)
    load_instr(pc++, instr_alui_d(1'b1, 3'd3, 3'd3, pack_subop(F_ADD), pack_imm27(to48(1))));   // D3 += 1

    load_instr(pc++, instr_sys(4'hF));                                                          // HALT

    sub_idx = 40;
    load_instr(sub_idx + 0, instr_alu(1'b0, 3'd6, 3'd0, 3'd6, F_ADD, 1'b0));                    // A6 = A0 + D6
    load_instr(sub_idx + 1, instr_jalr(1'b0, 3'd0, 3'd6, pack_imm33(to48(0))));                 // return via JALR (link to A0)

    // Patch branch and jump placeholders with proper offsets
    off = 15 - (idx_beq + 1);
    load_instr(idx_beq, instr_br(C_BEQ, 3'd1, 3'd1, pack_off31(to48(off))));

    off = 17 - (idx_bne + 1);
    load_instr(idx_bne, instr_br(C_BNE, 3'd1, 3'd1, pack_off31(to48(off))));

    off = 19 - (idx_blt + 1);
    load_instr(idx_blt, instr_br(C_BLT, 3'd2, 3'd2, pack_off31(to48(off))));

    off = 21 - (idx_bltu + 1);
    load_instr(idx_bltu, instr_br(C_BLTU, 3'd4, 3'd3, pack_off31(to48(off))));

    off = 23 - (idx_bge + 1);
    load_instr(idx_bge, instr_br(C_BGE, 3'd1, 3'd1, pack_off31(to48(off))));

    off = 25 - (idx_bgeu + 1);
    load_instr(idx_bgeu, instr_br(C_BGEU, 3'd4, 3'd3, pack_off31(to48(off))));

    off = loop_start - (idx_loop_branch + 1);
    load_instr(idx_loop_branch, instr_br(C_BNE, 3'd5, 3'd0, pack_off31(to48(off))));

    off = sub_idx - (idx_jal_call + 1);
    load_instr(idx_jal_call, instr_jal(1'b1, 3'd6, pack_off36(to48(off))));                     // D6 gets return addr

    off = 32 - (idx_jal_skip + 1);
    load_instr(idx_jal_skip, instr_jal(1'b0, 3'd7, pack_off36(to48(off))));                     // A7 gets return addr

    repeat (4) @(posedge clk);
    resetn = 1'b1;

    wait (dut.halt);
    @(posedge clk);

    expect_a(3'd0, 48'd0);
    expect_a(3'd1, to48(5));
    expect_a(3'd2, to48(-7));
    expect_a(3'd3, to48(8));
    expect_a(3'd4, to48(-1));
    expect_a(3'd5, to48(0));
    expect_a(3'd6, to48(idx_jal_call + 1));
    expect_a(3'd7, to48(idx_jal_skip + 1));

    expect_d(3'd0, to48(0));
    expect_d(3'd1, to48(5));
    expect_d(3'd2, to48(-3 + 42));
    expect_d(3'd3, to48(10));
    expect_d(3'd4, to48(1));
    expect_d(3'd5, to48(1));
    expect_d(3'd6, to48(idx_jal_call + 1));
    expect_d(3'd7, to48(3));

    $display("cpu_ad48_ctrl_tb PASS");
    $finish;
  end
endmodule
