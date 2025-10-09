`timescale 1ns/1ps
`ifdef CPU_AD48_INSTR_VH
`undef CPU_AD48_INSTR_VH
`endif

module cpu_ad48_mem_tb;
  localparam IM_WORDS = 128;
  localparam DM_WORDS = 128;

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
        $display("MEM TB: A%0d mismatch. Got %h expected %h", idx, dut.RF_A.regs[idx], value);
        $fatal(1);
      end
    end
  endtask

  task expect_d;
    input [2:0] idx;
    input [47:0] value;
    begin
      if (dut.RF_D.regs[idx] !== value) begin
        $display("MEM TB: D%0d mismatch. Got %h expected %h", idx, dut.RF_D.regs[idx], value);
        $fatal(1);
      end
    end
  endtask

  task expect_mem;
    input integer addr;
    input [47:0] value;
    begin
      if (dut.DMEM.mem[addr] !== value) begin
        $display("MEM TB: MEM[%0d] mismatch. Got %h expected %h", addr, dut.DMEM.mem[addr], value);
        $fatal(1);
      end
    end
  endtask

  integer i;
  integer pc;

  initial begin
    resetn = 1'b0;

    // Clear and preload memory images
    for (i = 0; i < IM_WORDS; i = i + 1) dut.IMEM.mem[i] = 48'd0;
    for (i = 0; i < DM_WORDS; i = i + 1) dut.DMEM.mem[i] = 48'd0;

    dut.DMEM.mem[0] = to48(100);
    dut.DMEM.mem[1] = to48(200);
    dut.DMEM.mem[2] = to48(300);
    dut.DMEM.mem[3] = to48(400);
    dut.DMEM.mem[4] = to48(500);
    dut.DMEM.mem[5] = to48(600);
    dut.DMEM.mem[6] = to48(700);
    dut.DMEM.mem[7] = to48(800);
    dut.DMEM.mem[8] = to48(900);

    pc = 0;
    load_instr(pc++, instr_ld(1'b0, 3'd0, 3'd0, pack_disp33(to48(0))));   // D0 <- MEM[0]
    load_instr(pc++, instr_ld(1'b0, 3'd1, 3'd0, pack_disp33(to48(1))));   // D1 <- MEM[1]
    load_instr(pc++, instr_alui_a(1'b0, 3'd1, 3'd1, pack_subop(F_ADD), pack_imm27(to48(2)))); // A1 = 2
    load_instr(pc++, instr_ld(1'b0, 3'd2, 3'd1, pack_disp33(to48(0))));   // D2 <- MEM[2]
    load_instr(pc++, instr_ld(1'b1, 3'd3, 3'd1, pack_disp33(to48(2))));   // D3 <- MEM[4], A1 += 2 -> 4
    load_instr(pc++, instr_ld(1'b0, 3'd4, 3'd1, pack_disp33(to48(-1))));  // D4 <- MEM[3]
    load_instr(pc++, instr_alui_d(1'b1, 3'd5, 3'd5, pack_subop(F_ADD), pack_imm27(to48(12345)))); // D5 = 12345
    load_instr(pc++, instr_st(1'b0, 3'd5, 3'd1, pack_disp33(to48(0))));   // MEM[4] = D5
    load_instr(pc++, instr_ld(1'b0, 3'd2, 3'd1, pack_disp33(to48(0))));   // D2 <- MEM[4] (=12345)
    load_instr(pc++, instr_st(1'b1, 3'd5, 3'd1, pack_disp33(to48(-2))));  // MEM[2] = D5, A1 += -2 -> 2
    load_instr(pc++, instr_alui_d(1'b1, 3'd6, 3'd6, pack_subop(F_ADD), pack_imm27(to48(67890)))); // D6 = 67890
    load_instr(pc++, instr_alui_a(1'b0, 3'd2, 3'd2, pack_subop(F_ADD), pack_imm27(to48(5)))); // A2 = 5
    load_instr(pc++, instr_st(1'b1, 3'd6, 3'd2, pack_disp33(to48(3))));   // MEM[8] = D6, A2 += 3 -> 8
    load_instr(pc++, instr_ld(1'b1, 3'd7, 3'd2, pack_disp33(to48(-2))));  // D7 <- MEM[6], A2 += -2 -> 6
    load_instr(pc++, instr_ld(1'b1, 3'd4, 3'd2, pack_disp33(to48(-1))));  // D4 <- MEM[5], A2 += -1 -> 5
    load_instr(pc++, instr_ld(1'b1, 3'd1, 3'd0, pack_disp33(to48(2))));   // D1 <- MEM[2], A0 postinc ignored
    load_instr(pc++, instr_sys(4'hF));                                    // HALT

    repeat (4) @(posedge clk);
    resetn = 1'b1;

    wait (dut.halt);
    @(posedge clk);

    expect_a(3'd0, 48'd0);             // A0 hard-wired
    expect_a(3'd1, to48(2));           // A1 updated via post-inc
    expect_a(3'd2, to48(5));           // A2 after mixed updates

    expect_d(3'd0, to48(100));
    expect_d(3'd1, to48(12345));
    expect_d(3'd2, to48(12345));
    expect_d(3'd3, to48(500));
    expect_d(3'd4, to48(600));
    expect_d(3'd5, to48(12345));
    expect_d(3'd6, to48(67890));
    expect_d(3'd7, to48(700));

    expect_mem(0, to48(100));
    expect_mem(1, to48(200));
    expect_mem(2, to48(12345));
    expect_mem(3, to48(400));
    expect_mem(4, to48(12345));
    expect_mem(5, to48(600));
    expect_mem(6, to48(700));
    expect_mem(7, to48(800));
    expect_mem(8, to48(67890));

    if (dut.RF_A.regs[0] !== 48'd0) begin
      $display("MEM TB: A0 should remain zero but read %h", dut.RF_A.regs[0]);
      $fatal(1);
    end

    $display("cpu_ad48_mem_tb PASS");
    $finish;
  end
endmodule
