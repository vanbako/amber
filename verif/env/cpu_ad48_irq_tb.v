`timescale 1ns/1ps
`ifdef CPU_AD48_INSTR_VH
`undef CPU_AD48_INSTR_VH
`endif

module cpu_ad48_irq_tb;
  localparam IM_WORDS = 128;
  localparam DM_WORDS = 32;
  localparam IRQ_LINES = 4;
  localparam integer TRAP_IDX = 48;
  localparam [47:0] TRAP_VEC = 48'd48;
  localparam integer IRQ_VEC_IDX = 40;
  localparam [47:0] IRQ_VEC = 48'd40;
  localparam [47:0] TARGET_PC = 48'd7;
  localparam integer STATUS_MIE_BIT = 6;

  reg clk;
  reg resetn;
  reg [IRQ_LINES-1:0] irq_lines;

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

  task init_mem;
    integer idx;
    begin
      for (idx = 0; idx < IM_WORDS; idx = idx + 1) begin
        dut.IMEM.mem[idx] = 48'd0;
      end
      for (idx = 0; idx < DM_WORDS; idx = idx + 1) begin
        dut.DMEM.mem[idx] = 48'd0;
      end
      dut.IMEM.mem[TRAP_IDX] = instr_sys(SYS_F_HALT);

      // Main program
      dut.IMEM.mem[0] = instr_alui_d(1'b1, 3'd1, 3'd1, pack_subop(F_ADD), pack_imm27(to48(48'h043))); // STATUS with MIE=1
      dut.IMEM.mem[1] = instr_csr   (CSR_F_RW, 1'b0, 3'd0, 3'd1, pack_csr_addr(CSR_ADDR_STATUS));
      dut.IMEM.mem[2] = instr_alui_d(1'b1, 3'd2, 3'd2, pack_subop(F_ADD), pack_imm27(to48(1)));     // IRQ enable mask (bit 0)
      dut.IMEM.mem[3] = instr_csr   (CSR_F_RW, 1'b0, 3'd0, 3'd2, pack_csr_addr(CSR_ADDR_IRQ_ENABLE));
      dut.IMEM.mem[4] = instr_alui_d(1'b1, 3'd3, 3'd3, pack_subop(F_ADD), pack_imm27(to48(IRQ_VEC_IDX)));
      dut.IMEM.mem[5] = instr_csr   (CSR_F_RW, 1'b0, 3'd0, 3'd3, pack_csr_addr(CSR_ADDR_IRQ_VECTOR));
      dut.IMEM.mem[6] = instr_sys   (SYS_F_NOP);
      dut.IMEM.mem[7] = instr_alui_d(1'b1, 3'd5, 3'd5, pack_subop(F_ADD), pack_imm27(to48(1)));     // Target instruction (should run once)
      dut.IMEM.mem[8] = instr_alui_d(1'b1, 3'd6, 3'd6, pack_subop(F_ADD), pack_imm27(to48(1)));
      dut.IMEM.mem[9] = instr_sys   (SYS_F_HALT);

      // IRQ handler at IRQ_VEC
      dut.IMEM.mem[IRQ_VEC_IDX + 0] = instr_alui_d(1'b1, 3'd1, 3'd0, pack_subop(F_ADD), pack_imm27(to48(1))); // D1 = 1 (mask)
      dut.IMEM.mem[IRQ_VEC_IDX + 1] = instr_csr   (CSR_F_RC, 1'b0, 3'd0, 3'd1, pack_csr_addr(CSR_ADDR_IRQ_PENDING)); // clear pending
      dut.IMEM.mem[IRQ_VEC_IDX + 2] = instr_sys   (SYS_F_IRET);
    end
  endtask

  task check_results;
    begin
      if (dut.csr_epc !== TARGET_PC) begin
        $display("IRQ TB: EPC mismatch. Got %h expected %h", dut.csr_epc, TARGET_PC);
        $fatal(1);
      end
      if (dut.csr_cause[47] !== 1'b1) begin
        $display("IRQ TB: CAUSE[47] not set for interrupt");
        $fatal(1);
      end
      if (dut.csr_cause[5:0] !== 6'd0) begin
        $display("IRQ TB: CAUSE index mismatch. Got %0d expected %0d", dut.csr_cause[5:0], 0);
        $fatal(1);
      end
      if (dut.csr_status[STATUS_MIE_BIT] !== 1'b1) begin
        $display("IRQ TB: STATUS[MIE] not restored. Got %b", dut.csr_status[STATUS_MIE_BIT]);
        $fatal(1);
      end
      if (dut.csr_irq_enable[0] !== 1'b1) begin
        $display("IRQ TB: IRQ enable mask lost. Got %b", dut.csr_irq_enable[0]);
        $fatal(1);
      end
      if (dut.csr_irq_pending[0] !== 1'b0) begin
        $display("IRQ TB: IRQ pending bit not cleared. Got %b", dut.csr_irq_pending[0]);
        $fatal(1);
      end
      if (dut.RF_D.regs[5] !== to48(1)) begin
        $display("IRQ TB: D5 mismatch. Got %h expected %h", dut.RF_D.regs[5], to48(1));
        $fatal(1);
      end
      if (dut.RF_D.regs[6] !== to48(1)) begin
        $display("IRQ TB: D6 mismatch. Got %h expected %h", dut.RF_D.regs[6], to48(1));
        $fatal(1);
      end
      if (dut.pc !== 48'd9) begin
        $display("IRQ TB: PC mismatch after HALT. Got %h expected %h", dut.pc, 48'd9);
        $fatal(1);
      end
    end
  endtask

  initial begin
    resetn = 1'b0;
    irq_lines = {IRQ_LINES{1'b0}};
    init_mem();
    repeat (4) @(posedge clk);
    resetn = 1'b1;

    wait (dut.pc == TARGET_PC);
    @(negedge clk);
    irq_lines[0] = 1'b1;
    @(posedge clk);
    irq_lines[0] = 1'b0;

    wait (dut.pc == IRQ_VEC);
    repeat (3) @(posedge clk);

    wait (dut.halt);
    @(posedge clk);

    check_results();

    $display("cpu_ad48_irq_tb PASS");
    $finish;
  end
endmodule
