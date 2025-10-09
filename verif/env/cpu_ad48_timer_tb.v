`timescale 1ns/1ps
`ifdef CPU_AD48_INSTR_VH
`undef CPU_AD48_INSTR_VH
`endif

module cpu_ad48_timer_tb;
  localparam IM_WORDS = 128;
  localparam DM_WORDS = 32;
  localparam IRQ_LINES = 4;
  localparam integer TIMER_IRQ_BIT = IRQ_LINES;
  localparam integer TOTAL_IRQ_LINES = IRQ_LINES + 1;
  localparam integer IRQ_INDEX_WIDTH = (TOTAL_IRQ_LINES <= 1) ? 1 : $clog2(TOTAL_IRQ_LINES);
  localparam integer TRAP_VEC_IDX = 48;
  localparam [47:0] TRAP_VEC = 48'd48;
  localparam integer IRQ_VEC_IDX = 28;
  localparam [47:0] IRQ_VEC = 48'd28;
  localparam integer HALT_PC = 27;
  localparam [47:0] TIMER_FIRST_CMP = 48'd12;
  localparam [47:0] TIMER_INTERVAL = 48'd64;
  localparam [47:0] TIMER_IRQ_MASK = (48'h1 << TIMER_IRQ_BIT);
  localparam integer TIMER_VEC_IDX = IRQ_VEC_IDX + TIMER_IRQ_BIT;

  reg clk;
  reg resetn;
  wire [IRQ_LINES-1:0] irq_lines = {IRQ_LINES{1'b0}};

  cpu_ad48 #(
    .IM_WORDS(IM_WORDS),
    .DM_WORDS(DM_WORDS),
    .TRAP_VECTOR(TRAP_VEC),
    .IRQ_LINES(IRQ_LINES),
    .IRQ_VECTOR(IRQ_VEC)
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

  initial begin
    repeat (2048) @(posedge clk);
    $display("Timer TB: Timeout. PC=%0d halt=%0b timer=%0d cmp=%0d pending=%h enable=%h cause=%h D6=%0d D7=%h",
             dut.pc, dut.halt, dut.csr_timer, dut.csr_timer_cmp, dut.csr_irq_pending,
             dut.csr_irq_enable, dut.csr_cause, dut.RF_D.regs[6], dut.RF_D.regs[7]);
    $fatal(1);
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
    end
  endtask

  task program_main;
    integer idx;
    begin
      init_mem();

      dut.IMEM.mem[TRAP_VEC_IDX] = instr_sys(SYS_F_HALT);

      // Main program
      idx = 0;
      dut.IMEM.mem[idx++] = instr_alui_d(1'b1, 3'd1, 3'd0, pack_subop(F_ADD), pack_imm27(to48(48'h13))); // STATUS: MIE=1, priv=machine
      dut.IMEM.mem[idx++] = instr_csr   (CSR_F_RW, 1'b0, 3'd0, 3'd1, pack_csr_addr(CSR_ADDR_STATUS));
      dut.IMEM.mem[idx++] = instr_alui_d(1'b1, 3'd7, 3'd0, pack_subop(F_ADD), pack_imm27(to48(48'h1 << TIMER_IRQ_BIT))); // mask for timer IRQ
      dut.IMEM.mem[idx++] = instr_csr   (CSR_F_RW, 1'b0, 3'd0, 3'd7, pack_csr_addr(CSR_ADDR_IRQ_ENABLE));
      dut.IMEM.mem[idx++] = instr_alui_d(1'b1, 3'd2, 3'd0, pack_subop(F_ADD), pack_imm27(to48(IRQ_VEC_IDX))); // IRQ vector base
      dut.IMEM.mem[idx++] = instr_csr   (CSR_F_RW, 1'b0, 3'd0, 3'd2, pack_csr_addr(CSR_ADDR_IRQ_VECTOR));
      dut.IMEM.mem[idx++] = instr_alui_d(1'b1, 3'd3, 3'd0, pack_subop(F_ADD), pack_imm27(to48(48'd0))); // seed timer with zero
      dut.IMEM.mem[idx++] = instr_csr   (CSR_F_RW, 1'b0, 3'd0, 3'd3, pack_csr_addr(CSR_ADDR_TIMER));
      dut.IMEM.mem[idx++] = instr_alui_d(1'b1, 3'd3, 3'd0, pack_subop(F_ADD), pack_imm27(to48(TIMER_FIRST_CMP)));
      dut.IMEM.mem[idx++] = instr_csr   (CSR_F_RW, 1'b0, 3'd0, 3'd3, pack_csr_addr(CSR_ADDR_TIMER_CMP));
      dut.IMEM.mem[idx++] = instr_csr   (CSR_F_R,  1'b1, 3'd4, 3'd0, pack_csr_addr(CSR_ADDR_TIMER)); // capture timer prior to wait loop

      repeat (16) begin
        dut.IMEM.mem[idx++] = instr_sys(SYS_F_NOP);
      end
      dut.IMEM.mem[idx++] = instr_sys(SYS_F_HALT);

      if (idx != HALT_PC + 1) begin
        $fatal(1, "Timer TB: unexpected HALT PC placement (idx=%0d expected=%0d)", idx-1, HALT_PC);
      end

      // IRQ handler at IRQ_VECTOR
      dut.IMEM.mem[TIMER_VEC_IDX + 0] = instr_csr   (CSR_F_R,  1'b1, 3'd1, 3'd0, pack_csr_addr(CSR_ADDR_TIMER)); // D1 = current timer
      dut.IMEM.mem[TIMER_VEC_IDX + 1] = instr_alui_d(1'b1, 3'd6, 3'd6, pack_subop(F_ADD), pack_imm27(to48(48'd1))); // increment tick count
      dut.IMEM.mem[TIMER_VEC_IDX + 2] = instr_alui_d(1'b1, 3'd3, 3'd1, pack_subop(F_ADD), pack_imm27(to48(TIMER_INTERVAL))); // D3 = timer + interval
      dut.IMEM.mem[TIMER_VEC_IDX + 3] = instr_csr   (CSR_F_RW, 1'b0, 3'd0, 3'd3, pack_csr_addr(CSR_ADDR_TIMER_CMP)); // re-arm compare
      dut.IMEM.mem[TIMER_VEC_IDX + 4] = instr_csr   (CSR_F_RC, 1'b0, 3'd0, 3'd7, pack_csr_addr(CSR_ADDR_IRQ_PENDING)); // clear timer pending
      dut.IMEM.mem[TIMER_VEC_IDX + 5] = instr_sys   (SYS_F_IRET);

    end
  endtask

  task check_results;
    reg [47:0] expected_cmp;
  begin
    if (dut.pc !== to48(HALT_PC)) begin
      $display("Timer TB: PC mismatch. Got %h expected %h", dut.pc, to48(HALT_PC));
      $fatal(1);
    end

    if (dut.RF_D.regs[6] !== to48(48'd1)) begin
      $display("Timer TB: Tick counter mismatch. D6=%h expected 1", dut.RF_D.regs[6]);
      $fatal(1);
    end

    if (dut.RF_D.regs[4] >= to48(TIMER_FIRST_CMP)) begin
      $display("Timer TB: Early timer capture too large. D4=%h threshold=%h",
               dut.RF_D.regs[4], to48(TIMER_FIRST_CMP));
      $fatal(1);
    end

    if (dut.csr_cause[47] !== 1'b1) begin
      $display("Timer TB: CSR_CAUSE[47] not set for interrupt. cause=%h", dut.csr_cause);
      $fatal(1);
    end

    if (dut.csr_cause[IRQ_INDEX_WIDTH-1:0] !== TIMER_IRQ_BIT[IRQ_INDEX_WIDTH-1:0]) begin
      $display("Timer TB: CSR_CAUSE index mismatch. Got %0d expected %0d",
               dut.csr_cause[IRQ_INDEX_WIDTH-1:0], TIMER_IRQ_BIT);
      $fatal(1);
    end

    if (dut.csr_irq_enable[TIMER_IRQ_BIT] !== 1'b1) begin
      $display("Timer TB: IRQ enable lost. csr_irq_enable=%h", dut.csr_irq_enable);
      $fatal(1);
    end

    if (dut.csr_irq_pending[TIMER_IRQ_BIT] !== 1'b0) begin
      $display("Timer TB: Timer pending bit still set. csr_irq_pending=%h", dut.csr_irq_pending);
      $fatal(1);
    end

    expected_cmp = dut.RF_D.regs[1] + to48(TIMER_INTERVAL);
    if (dut.csr_timer_cmp !== expected_cmp) begin
      $display("Timer TB: TIMER_CMP mismatch. Got %h expected %h (D1=%h interval=%h)",
               dut.csr_timer_cmp, expected_cmp, dut.RF_D.regs[1], to48(TIMER_INTERVAL));
      $fatal(1);
    end

    if (dut.csr_timer < to48(TIMER_FIRST_CMP)) begin
      $display("Timer TB: TIMER did not advance past initial compare. timer=%h", dut.csr_timer);
      $fatal(1);
    end
  end
  endtask

  initial begin
    resetn = 1'b0;
    program_main();

    repeat (4) @(posedge clk);
    resetn = 1'b1;

    wait (dut.halt);
    @(posedge clk);

    check_results();

    $display("cpu_ad48_timer_tb PASS");
    $finish;
  end
endmodule
