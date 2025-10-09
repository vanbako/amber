`timescale 1ns/1ps
`ifdef CPU_AD48_INSTR_VH
`undef CPU_AD48_INSTR_VH
`endif

module cpu_ad48_exc_tb;
  localparam IM_WORDS = 128;
  localparam DM_WORDS = 16;
  localparam integer TRAP_IDX = 32;
  localparam [47:0] TRAP_VEC = 48'd32;
  localparam IRQ_LINES = 4;

  reg clk;
  reg resetn;
  wire [IRQ_LINES-1:0] irq_lines = {IRQ_LINES{1'b0}};

  cpu_ad48 #(
    .IM_WORDS(IM_WORDS),
    .DM_WORDS(DM_WORDS),
    .TRAP_VECTOR(TRAP_VEC),
    .IRQ_LINES(IRQ_LINES)
  ) dut (
    .clk   (clk),
    .resetn(resetn),
    .irq   (irq_lines)
  );

  `include "../../src/rtl/cpu_ad48_instr.vh"

  localparam [3:0] CAUSE_ILLEGAL_INSTR    = 4'd2;
  localparam [3:0] CAUSE_BREAKPOINT       = 4'd3;
  localparam [3:0] CAUSE_MISALIGNED_LOAD  = 4'd4;
  localparam [3:0] CAUSE_MISALIGNED_STORE = 4'd6;
  localparam [1:0] PRIV_MACHINE           = 2'd3;

  initial begin
    clk = 1'b0;
    forever #5 clk = ~clk;
  end

  task init_mem;
    integer idx;
    begin
      for (idx = 0; idx < IM_WORDS; idx = idx + 1) begin
        dut.IMEM.mem[idx] = 48'd0;
      end
      for (idx = 0; idx < DM_WORDS; idx = idx + 1) begin
        dut.DMEM.mem[idx] = 48'd0;
      end
      dut.IMEM.mem[TRAP_IDX] = instr_sys(4'hF);
    end
  endtask

  task wait_for_trap;
    integer cycle_count;
    begin
      cycle_count = 0;
      while (!dut.halt && cycle_count < 200) begin
        @(posedge clk);
        cycle_count = cycle_count + 1;
      end
      if (!dut.halt) begin
        $display("EXC TB: trap wait timeout");
        $fatal(1);
      end
      @(posedge clk);
    end
  endtask

  task check_trap;
    input [47:0] exp_epc;
    input [3:0]  exp_cause;
    input [47:0] exp_instret;
    begin
      if (dut.pc !== TRAP_VEC) begin
        $display("EXC TB: PC mismatch. Got %h expected %h", dut.pc, TRAP_VEC);
        $fatal(1);
      end
      if (dut.csr_epc !== exp_epc) begin
        $display("EXC TB: EPC mismatch. Got %h expected %h", dut.csr_epc, exp_epc);
        $fatal(1);
      end
      if (dut.csr_cause[3:0] !== exp_cause) begin
        $display("EXC TB: CAUSE mismatch. Got %0d expected %0d", dut.csr_cause[3:0], exp_cause);
        $fatal(1);
      end
      if (dut.priv_mode !== PRIV_MACHINE) begin
        $display("EXC TB: priv_mode mismatch. Got %0d expected %0d", dut.priv_mode, PRIV_MACHINE);
        $fatal(1);
      end
      if (dut.csr_status[1:0] !== PRIV_MACHINE) begin
        $display("EXC TB: STATUS[1:0] mismatch. Got %0d expected %0d", dut.csr_status[1:0], PRIV_MACHINE);
        $fatal(1);
      end
      if (dut.csr_instret !== exp_instret) begin
        $display("EXC TB: INSTRET mismatch. Got %h expected %h", dut.csr_instret, exp_instret);
        $fatal(1);
      end
    end
  endtask

  task run_illegal;
    begin
      resetn = 1'b0;
      init_mem();
      dut.IMEM.mem[0] = instr_alu(1'b0, 3'd1, 3'd1, 3'd1, 4'hF, 1'b0); // invalid funct => illegal instruction
      repeat (4) @(posedge clk);
      resetn = 1'b1;
      wait_for_trap();
      check_trap(48'd0, CAUSE_ILLEGAL_INSTR, 48'd0);
    end
  endtask

  task run_breakpoint;
    begin
      resetn = 1'b0;
      init_mem();
      dut.IMEM.mem[0] = instr_sys(4'h1); // breakpoint
      repeat (4) @(posedge clk);
      resetn = 1'b1;
      wait_for_trap();
      check_trap(48'd0, CAUSE_BREAKPOINT, 48'd0);
    end
  endtask

  task run_misaligned_load;
    begin
      resetn = 1'b0;
      init_mem();
      // A1 = 0
      dut.IMEM.mem[0] = instr_alui_a(1'b0, 3'd1, 3'd0, pack_subop(F_ADD), pack_imm27(to48(0)));
      // Load from address DM_WORDS -> out-of-range/misaligned
      dut.IMEM.mem[1] = instr_ld(1'b0, 3'd1, 3'd1, pack_disp33(to48(DM_WORDS)));
      repeat (4) @(posedge clk);
      resetn = 1'b1;
      wait_for_trap();
      check_trap(48'd1, CAUSE_MISALIGNED_LOAD, 48'd1);
    end
  endtask

  task run_misaligned_store;
    begin
      resetn = 1'b0;
      init_mem();
      // A1 = 0
      dut.IMEM.mem[0] = instr_alui_a(1'b0, 3'd1, 3'd0, pack_subop(F_ADD), pack_imm27(to48(0)));
      // D1 = 5
      dut.IMEM.mem[1] = instr_alui_d(1'b1, 3'd1, 3'd0, pack_subop(F_ADD), pack_imm27(to48(5)));
      // Store to address DM_WORDS -> out-of-range/misaligned
      dut.IMEM.mem[2] = instr_st(1'b0, 3'd1, 3'd1, pack_disp33(to48(DM_WORDS)));
      repeat (4) @(posedge clk);
      resetn = 1'b1;
      wait_for_trap();
      check_trap(48'd2, CAUSE_MISALIGNED_STORE, 48'd2);
    end
  endtask

  initial begin
    run_illegal();
    run_breakpoint();
    run_misaligned_load();
    run_misaligned_store();
    $display("cpu_ad48_exc_tb PASS");
    $finish;
  end
endmodule
