`timescale 1ns/1ps
`ifdef CPU_AD48_INSTR_VH
`undef CPU_AD48_INSTR_VH
`endif

module cpu_ad48_priv_tb;
  localparam IM_WORDS  = 256;
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

  localparam [3:0] CAUSE_ILLEGAL_INSTR    = 4'd2;
  localparam [1:0] PRIV_USER              = 2'd0;
  localparam [1:0] PRIV_SUPERVISOR        = 2'd1;
  localparam [1:0] PRIV_MACHINE           = 2'd3;
  localparam integer STATUS_PREV_MODE_LSB = 2;
  localparam integer STATUS_PREV_MODE_MSB = 3;
  localparam integer STATUS_UIE_BIT       = 4;
  localparam integer STATUS_KIE_BIT       = 5;
  localparam integer STATUS_MIE_BIT       = 6;
  localparam integer STATUS_UPIE_BIT      = 7;
  localparam integer STATUS_KPIE_BIT      = 8;
  localparam integer STATUS_MPIE_BIT      = 9;

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

  integer pc;

  initial begin
    resetn = 1'b0;
    for (pc = 0; pc < IM_WORDS; pc = pc + 1) begin
      dut.IMEM.mem[pc] = 48'd0;
    end

    // Main program at PC=0.
    pc = 0;
    // D1 <= 0 (ANDI with 0).
    load_instr(pc++, instr_alui_d(1'b1, 3'd1, 3'd1, pack_subop(F_AND), pack_imm27(to48(48'd0))));
    // D1 <= 0x73 (UIE/KIE/MIE set, machine mode).
    load_instr(pc++, instr_alui_d(1'b1, 3'd1, 3'd1, pack_subop(F_ADD), pack_imm27(to48(48'h073))));
    load_instr(pc++, instr_csr(CSR_F_RW, 1'b0, 3'd0, 3'd1, pack_csr_addr(CSR_ADDR_STATUS)));
    // D1 <= 0.
    load_instr(pc++, instr_alui_d(1'b1, 3'd1, 3'd1, pack_subop(F_AND), pack_imm27(to48(48'd0))));
    // D1 <= 0x71 (drop to supervisor, keep IE bits).
    load_instr(pc++, instr_alui_d(1'b1, 3'd1, 3'd1, pack_subop(F_ADD), pack_imm27(to48(48'h071))));
    load_instr(pc++, instr_csr(CSR_F_RW, 1'b0, 3'd0, 3'd1, pack_csr_addr(CSR_ADDR_STATUS)));
    // D1 <= 0.
    load_instr(pc++, instr_alui_d(1'b1, 3'd1, 3'd1, pack_subop(F_AND), pack_imm27(to48(48'd0))));
    // D1 <= 0x74 (enter user mode, remember supervisor in PREV field).
    load_instr(pc++, instr_alui_d(1'b1, 3'd1, 3'd1, pack_subop(F_ADD), pack_imm27(to48(48'h074))));
    load_instr(pc++, instr_csr(CSR_F_RW, 1'b0, 3'd0, 3'd1, pack_csr_addr(CSR_ADDR_STATUS)));
    // D2 <= 0.
    load_instr(pc++, instr_alui_d(1'b1, 3'd2, 3'd2, pack_subop(F_AND), pack_imm27(to48(48'd0))));
    // D2 <= 1 (attempt to program scratch from user mode -> should fault).
    load_instr(pc++, instr_alui_d(1'b1, 3'd2, 3'd2, pack_subop(F_ADD), pack_imm27(to48(48'd1))));
    load_instr(pc++, instr_csr(CSR_F_RW, 1'b0, 3'd0, 3'd2, pack_csr_addr(CSR_ADDR_SCRATCH)));
    // Instructions executed after IRET resumes execution at PC=12.
    load_instr(pc++, instr_csr(CSR_F_R, 1'b1, 3'd3, 3'd0, pack_csr_addr(CSR_ADDR_CYCLE)));
    load_instr(pc++, instr_sys(SYS_F_HALT));

    // Trap handler at TRAP_VECTOR (default 64).
    pc = 64;
    load_instr(pc++, instr_csr(CSR_F_R, 1'b1, 3'd4, 3'd0, pack_csr_addr(CSR_ADDR_STATUS)));
    load_instr(pc++, instr_csr(CSR_F_R, 1'b1, 3'd5, 3'd0, pack_csr_addr(CSR_ADDR_CAUSE)));
    load_instr(pc++, instr_csr(CSR_F_R, 1'b1, 3'd6, 3'd0, pack_csr_addr(CSR_ADDR_EPC)));
    load_instr(pc++, instr_csr(CSR_F_R, 1'b1, 3'd7, 3'd0, pack_csr_addr(CSR_ADDR_SCRATCH)));
    // D0 <= 0 then add 12 to set EPC target.
    load_instr(pc++, instr_alui_d(1'b1, 3'd0, 3'd0, pack_subop(F_AND), pack_imm27(to48(48'd0))));
    load_instr(pc++, instr_alui_d(1'b1, 3'd0, 3'd0, pack_subop(F_ADD), pack_imm27(to48(48'd12))));
    load_instr(pc++, instr_csr(CSR_F_RW, 1'b0, 3'd0, 3'd0, pack_csr_addr(CSR_ADDR_EPC)));
    load_instr(pc++, instr_sys(SYS_F_IRET));

    repeat (5) @(posedge clk);
    resetn = 1'b1;

    wait (dut.halt);
    @(posedge clk);

    // Trap captured state.
    if (dut.RF_D.regs[4][1:0] !== PRIV_MACHINE) begin
      $display("PRIV TB: STATUS trap mode mismatch. Got %0d expected %0d", dut.RF_D.regs[4][1:0], PRIV_MACHINE);
      $fatal(1);
    end
    if (dut.RF_D.regs[4][STATUS_PREV_MODE_MSB:STATUS_PREV_MODE_LSB] !== PRIV_USER) begin
      $display("PRIV TB: STATUS trap previous mode mismatch. Got %0d expected %0d",
               dut.RF_D.regs[4][STATUS_PREV_MODE_MSB:STATUS_PREV_MODE_LSB], PRIV_USER);
      $fatal(1);
    end
    if (dut.RF_D.regs[4][STATUS_MIE_BIT] !== 1'b0) begin
      $display("PRIV TB: STATUS trap MIE not cleared. value=%b", dut.RF_D.regs[4][STATUS_MIE_BIT]);
      $fatal(1);
    end
    if (dut.RF_D.regs[4][STATUS_MPIE_BIT] !== 1'b1) begin
      $display("PRIV TB: STATUS trap MPIE not preserved. value=%b", dut.RF_D.regs[4][STATUS_MPIE_BIT]);
      $fatal(1);
    end
    if (dut.RF_D.regs[4][STATUS_UIE_BIT] !== 1'b1) begin
      $display("PRIV TB: STATUS trap UIE lost. value=%b", dut.RF_D.regs[4][STATUS_UIE_BIT]);
      $fatal(1);
    end
    if (dut.RF_D.regs[4][STATUS_KIE_BIT] !== 1'b1) begin
      $display("PRIV TB: STATUS trap KIE lost. value=%b", dut.RF_D.regs[4][STATUS_KIE_BIT]);
      $fatal(1);
    end

    if (dut.RF_D.regs[5][3:0] !== CAUSE_ILLEGAL_INSTR) begin
      $display("PRIV TB: CAUSE mismatch. Got %0d expected %0d", dut.RF_D.regs[5][3:0], CAUSE_ILLEGAL_INSTR);
      $fatal(1);
    end

    if (dut.RF_D.regs[6] !== to48(48'd11)) begin
      $display("PRIV TB: EPC mismatch. Got %h expected %h", dut.RF_D.regs[6], to48(48'd11));
      $fatal(1);
    end

    if (dut.RF_D.regs[7] !== 48'd0) begin
      $display("PRIV TB: SCRATCH modified unexpectedly. value=%h", dut.RF_D.regs[7]);
      $fatal(1);
    end

    // Post-IRET state: returned to user mode with interrupt enables restored.
    if (dut.priv_mode !== PRIV_USER) begin
      $display("PRIV TB: priv_mode after IRET mismatch. Got %0d expected %0d", dut.priv_mode, PRIV_USER);
      $fatal(1);
    end
    if (dut.csr_status[1:0] !== PRIV_USER) begin
      $display("PRIV TB: STATUS current mode mismatch post-IRET. Got %0d expected %0d", dut.csr_status[1:0], PRIV_USER);
      $fatal(1);
    end
    if (dut.csr_status[STATUS_PREV_MODE_MSB:STATUS_PREV_MODE_LSB] !== PRIV_USER) begin
      $display("PRIV TB: STATUS prev mode not cleared post-IRET. Got %0d expected %0d",
               dut.csr_status[STATUS_PREV_MODE_MSB:STATUS_PREV_MODE_LSB], PRIV_USER);
      $fatal(1);
    end
    if (dut.csr_status[STATUS_MIE_BIT] !== 1'b1) begin
      $display("PRIV TB: STATUS MIE not restored post-IRET. value=%b", dut.csr_status[STATUS_MIE_BIT]);
      $fatal(1);
    end
    if (dut.csr_status[STATUS_UIE_BIT] !== 1'b1) begin
      $display("PRIV TB: STATUS UIE not maintained post-IRET. value=%b", dut.csr_status[STATUS_UIE_BIT]);
      $fatal(1);
    end
    if (dut.csr_status[STATUS_KIE_BIT] !== 1'b1) begin
      $display("PRIV TB: STATUS KIE not maintained post-IRET. value=%b", dut.csr_status[STATUS_KIE_BIT]);
      $fatal(1);
    end
    if (dut.csr_status[STATUS_UPIE_BIT] !== 1'b1) begin
      $display("PRIV TB: STATUS UPIE not set post-IRET. value=%b", dut.csr_status[STATUS_UPIE_BIT]);
      $fatal(1);
    end
    if (dut.csr_status[STATUS_MPIE_BIT] !== 1'b1) begin
      $display("PRIV TB: STATUS MPIE not retained post-IRET. value=%b", dut.csr_status[STATUS_MPIE_BIT]);
      $fatal(1);
    end
    if (dut.csr_scratch !== 48'd0) begin
      $display("PRIV TB: SCRATCH modified by user write. value=%h", dut.csr_scratch);
      $fatal(1);
    end

    $display("cpu_ad48_priv_tb PASS");
    $finish;
  end
endmodule
