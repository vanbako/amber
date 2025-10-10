`timescale 1ns/1ps

// ============================================================================
//  cpu_ad48: single-cycle 48-bit Harvard core with split address/data banks.
//  - A bank (8 regs): A0 hard-wired to zero; typically holds base pointers. Post-inc never updates A0.
//  - D bank (8 regs): general-purpose data. Loads always write to D, stores read from D.
//  - Word-addressed (48-bit words). PC counts words. IMEM/DMEM realised with simple_mem48.
//  - Synchronous memories; every instruction completes in one cycle unless SYS.HALT holds the PC.
//  - Load/store support optional post-increment of the A base register.
//  - CSR block tracks privilege (user/supervisor/machine) and implements STATUS, SCRATCH, EPC, CAUSE, CYCLE, INSTRET.
//  - CSR operations source their write data from D, return the selected value to either bank, and enforce privilege checks.
//
// Instruction formats (48-bit, MSB..LSB). Reserved fields must be written as zero.
// 1) ALU (rd <= f(A[rsA], D[rsD]), optional operand swap):
//    [47:44]=OP_ALU
//    [43:40]=funct (ADD/SUB/AND/OR/XOR/SLL/SRL/SRA/NOT)
//    [39]=rdBank (0=A, 1=D), [38:36]=rdIdx, [35:33]=rsA, [32:30]=rsD
//    [29]=swap (1 swaps operand order), [28:24]=0, [23:0]=0
// 2) ALU-IMM A (operate on A source with sign-extended imm27/shamt):
//    [47:44]=OP_ALUI_A, [43:40]=0
//    [39]=rdBank, [38:36]=rdIdx, [35:33]=rsA
//    [32:27]=subop (bits[5:4]=0, bits[3:0] select ADD/AND/OR/XOR/SLL/SRL/SRA/NOT)
//    [26:0]=imm27 (sign-extended; low 6 bits drive shamt)
// 3) ALU-IMM D (operate on D source with imm27, same subop encoding as ALUI_A):
//    [47:44]=OP_ALUI_D, [43:40]=0
//    [39]=rdBank, [38:36]=rdIdx, [35:33]=rsD, [32:27]=subop, [26:0]=imm27
// 4) LOAD (rdD <= DMEM[A[baseA] + disp]):
//    [47:44]=OP_LD, [43:40]=0, [39]=post_inc, [38:36]=rdD, [35:33]=baseA, [32:0]=disp33
// 5) STORE (DMEM[A[baseA] + disp] <= D[rsD]):
//    [47:44]=OP_ST, [43:40]=0, [39]=post_inc, [38:36]=rsD, [35:33]=baseA, [32:0]=disp33
// 6) BRANCH (PC <= PC + 1 + off when condition on A vs D holds; offsets are word-relative):
//    [47:44]=OP_BR, [43:40]=0
//    [39:37]=cond (BEQ/BNE/BLT/BLTU/BGE/BGEU/ALWAYS), [36:34]=rsA, [33:31]=rsD, [30:0]=off31
// 7) JAL (link <= PC + 1; PC <= PC + 1 + off):
//    [47:44]=OP_JAL, [43:40]=0, [39]=rdBank, [38:36]=rdIdx, [35:0]=off36
// 8) JALR (link <= PC + 1; PC <= A[rsA] + imm):
//    [47:44]=OP_JALR, [43:40]=0, [39]=rdBank, [38:36]=rdIdx, [35:33]=rsA, [32:0]=imm33
// 9) CSR (read/modify CSRs with privilege enforcement):
//    [47:44]=OP_CSR, [43:40]=funct (RW/RS/RC/R), [39]=rdBank, [38:36]=rdIdx
//    [35:33]=0 (reserved), [32:30]=rsD (source operand), [29:24]=0, [23:12]=csr_addr, [11:0]=0
// 10) SYS (system functions; only HALT implemented, funct=0xF holds PC):
//    [47:44]=OP_SYS, [43:40]=funct, [39:0]=0
//
// Notes:
//  - All immediates/offsets are sign-extended to 48 bits.
//  - Word-addressed: offsets/disp count 48-bit words; memories operate without byte enables.
//  - A0 always reads as 0; writes to A0 ignored; post-inc suppressed when base==A0.
//  - CSR_STATUS[1:0] mirrors priv_mode (user/supervisor/machine); CYCLE increments every cycle, INSTRET increments on retired instructions.
//
// ============================================================================
module cpu_ad48 #(
  parameter IM_WORDS = 16384,
  parameter DM_WORDS = 16384,
  parameter IRQ_LINES = 4,
  parameter [47:0] TRAP_VECTOR = 48'd64,
  parameter [47:0] IRQ_VECTOR = 48'd128
)(
  input                    clk,
  input                    resetn,
  input  [IRQ_LINES-1:0]   irq
);
  // Reset include guard so compile order doesn't drop instruction constants.
`ifdef CPU_AD48_INSTR_VH
`undef CPU_AD48_INSTR_VH
`endif
  `include "cpu_ad48_instr.vh"

  // Opcode, funct, and branch encodings are provided by cpu_ad48_instr.vh.

  // ------------------- PC & IMEM -------------------
  reg  [47:0] pc;
  wire [47:0] instr;

  wire [$clog2(IM_WORDS)-1:0] pc_idx = pc[$clog2(IM_WORDS)-1:0];

  // IMEM instance (read-only here). Initialize externally with $readmemh.
  simple_mem48 #(.WORDS(IM_WORDS)) IMEM (
    .clk(clk), .addr(pc_idx), .we(1'b0), .wdata(48'd0), .rdata(instr)
  );

  // ------------------- Decode common fields -------------------
  wire [3:0]  op      = instr[47:44];
  wire [3:0]  op_ext  = instr[43:40];

  // ALU
  wire        rdBankA = (instr[39]==1'b0);
  wire [2:0]  rdIdx   = instr[38:36];
  wire [2:0]  rsA     = instr[35:33];
  wire [2:0]  rsD     = instr[32:30];
  wire [2:0]  rsD_im  = instr[35:33]; // ALUI_D source selector
  wire        swap    = (op == OP_ALU) ? instr[29] : 1'b0;

  // ALUI_A / ALUI_D
  wire [5:0]  subop   = instr[32:27];
  wire [26:0] imm27   = instr[26:0];

  // LD/ST
  wire        postinc = instr[39];
  wire [2:0]  ld_rdD  = instr[38:36];
  wire [2:0]  st_rsD  = instr[38:36];
  wire [2:0]  baseA   = instr[35:33];
  wire [32:0] disp33  = instr[32:0];

  // BR
  wire [2:0]  br_cond = instr[39:37];
  wire [2:0]  br_rsA  = instr[36:34];
  wire [2:0]  br_rsD  = instr[33:31];
  wire [30:0] br_off31= instr[30:0];

  // JAL / JALR
  wire        j_rdBank= instr[39];
  wire [2:0]  j_rdIdx = instr[38:36];
  wire [35:0] jal_off = instr[35:0];
  wire [2:0]  jr_rsA  = instr[35:33];
  wire [32:0] jr_imm33= instr[32:0];
  wire [11:0] csr_addr= instr[23:12];

  // ------------------- CSR constants & state -------------------
  localparam [1:0] PRIV_USER       = 2'd0;
  localparam [1:0] PRIV_SUPERVISOR = 2'd1;
  localparam [1:0] PRIV_MACHINE    = 2'd3;

  localparam [11:0] CSR_STATUS     = 12'h000;
  localparam [11:0] CSR_SCRATCH    = 12'h001;
  localparam [11:0] CSR_EPC        = 12'h002;
  localparam [11:0] CSR_CAUSE      = 12'h003;
  localparam [11:0] CSR_LR         = 12'h004;
  localparam [11:0] CSR_SSP        = 12'h005;
  localparam [11:0] CSR_IRQ_ENABLE = 12'h010;
  localparam [11:0] CSR_IRQ_PENDING= 12'h011;
  localparam [11:0] CSR_IRQ_VECTOR = 12'h012;
  localparam [11:0] CSR_CYCLE      = 12'hC00;
  localparam [11:0] CSR_INSTRET    = 12'hC01;
  localparam [11:0] CSR_TIMER      = 12'hC02;
  localparam [11:0] CSR_TIMER_CMP  = 12'hC03;
  localparam integer TIMER_IRQ_BIT = IRQ_LINES;
  localparam integer TOTAL_IRQ_LINES = IRQ_LINES + 1;
  localparam integer IRQ_INDEX_WIDTH = (TOTAL_IRQ_LINES <= 1) ? 1 : $clog2(TOTAL_IRQ_LINES);
  localparam [47:0] TIMER_IRQ_BIT_MASK = (TIMER_IRQ_BIT >= 48) ? 48'd0 : (48'h1 << TIMER_IRQ_BIT);
  localparam [47:0] IRQ_LINE_MASK = (TOTAL_IRQ_LINES >= 48) ? {48{1'b1}} : ((48'h1 << TOTAL_IRQ_LINES) - 1);
  localparam integer STATUS_PREV_MODE_LSB = 2;
  localparam integer STATUS_PREV_MODE_MSB = 3;
  localparam integer STATUS_UIE_BIT       = 4;
  localparam integer STATUS_KIE_BIT       = 5;
  localparam integer STATUS_MIE_BIT       = 6;
  localparam integer STATUS_UPIE_BIT      = 7;
  localparam integer STATUS_KPIE_BIT      = 8;
  localparam integer STATUS_MPIE_BIT      = 9;

  // CSR metadata encodings
  localparam [47:0] CSR_MASK_ZERO        = 48'd0;
  localparam [47:0] CSR_MASK_FULL        = {48{1'b1}};
  localparam [47:0] CSR_MASK_STATUS      = CSR_MASK_FULL;
  localparam [3:0] CSR_SEL_ZERO       = 4'd0;
  localparam [3:0] CSR_SEL_STATUS     = 4'd1;
  localparam [3:0] CSR_SEL_SCRATCH    = 4'd2;
  localparam [3:0] CSR_SEL_EPC        = 4'd3;
  localparam [3:0] CSR_SEL_CAUSE      = 4'd4;
  localparam [3:0] CSR_SEL_LR         = 4'd5;
  localparam [3:0] CSR_SEL_SSP        = 4'd6;
  localparam [3:0] CSR_SEL_IRQ_ENABLE = 4'd7;
  localparam [3:0] CSR_SEL_IRQ_PENDING= 4'd8;
  localparam [3:0] CSR_SEL_IRQ_VECTOR = 4'd9;
  localparam [3:0] CSR_SEL_CYCLE      = 4'd10;
  localparam [3:0] CSR_SEL_INSTRET    = 4'd11;
  localparam [3:0] CSR_SEL_TIMER      = 4'd12;
  localparam [3:0] CSR_SEL_TIMER_CMP  = 4'd13;

  localparam integer CSR_META_PRIV_WIDTH    = 2;
  localparam integer CSR_META_READSEL_WIDTH = 4;
  localparam integer CSR_META_WRITE_MASK_WIDTH = 48;
  localparam integer CSR_META_WIDTH         = 1 + CSR_META_PRIV_WIDTH + CSR_META_READSEL_WIDTH + 1 + CSR_META_WRITE_MASK_WIDTH;
  localparam integer CSR_META_WRITE_MASK_LSB = 0;
  localparam integer CSR_META_WRITE_MASK_MSB = CSR_META_WRITE_MASK_LSB + CSR_META_WRITE_MASK_WIDTH - 1;
  localparam integer CSR_META_WRITE_SIDE_BIT = CSR_META_WRITE_MASK_MSB + 1;
  localparam integer CSR_META_READSEL_LSB   = CSR_META_WRITE_SIDE_BIT + 1;
  localparam integer CSR_META_READSEL_MSB   = CSR_META_READSEL_LSB + CSR_META_READSEL_WIDTH - 1;
  localparam integer CSR_META_PRIV_LSB      = CSR_META_READSEL_MSB + 1;
  localparam integer CSR_META_PRIV_MSB      = CSR_META_PRIV_LSB + CSR_META_PRIV_WIDTH - 1;
  localparam integer CSR_META_VALID_BIT     = CSR_META_PRIV_MSB + 1;

`include "cpu_ad48_utils.vh"

  // Sign-extends
  wire [47:0] SX_imm27  = cpu_ad48_sx_imm27(imm27);
  wire [47:0] SX_disp33 = cpu_ad48_sx_disp33(disp33);
  wire [47:0] SX_br31   = cpu_ad48_sx_br31(br_off31);
  wire [47:0] SX_jal36  = cpu_ad48_sx_jal36(jal_off);
  wire [47:0] SX_jr33   = cpu_ad48_sx_jr33(jr_imm33);
  wire [48:0] jalr_target_ext = {1'b0, rA} + {SX_jr33[47], SX_jr33};
  wire [47:0] jalr_target     = jalr_target_ext[47:0];
  wire        jalr_target_ovf = jalr_target_ext[48];
  wire        jalr_target_oob = jalr_target_ovf || (jalr_target >= IM_WORDS);

  wire [48:0] base_plus_disp_ext = {1'b0, rA} + {SX_disp33[47], SX_disp33};
  wire [47:0] base_plus_disp     = base_plus_disp_ext[47:0];
  wire        base_plus_disp_ovf = base_plus_disp_ext[48];
  wire        base_plus_disp_oob = (base_plus_disp >= DM_WORDS);
  wire        mem_addr_invalid   = base_plus_disp_ovf || base_plus_disp_oob;
  wire [$clog2(DM_WORDS)-1:0] base_disp_idx = base_plus_disp[$clog2(DM_WORDS)-1:0];

  initial begin
    if (TOTAL_IRQ_LINES > 48) begin
      $fatal(1, "cpu_ad48: TOTAL_IRQ_LINES (%0d) exceeds supported width (48)", TOTAL_IRQ_LINES);
    end
  end

  reg [1:0]  priv_mode;
  reg [47:0] csr_status;
  reg [47:0] csr_scratch;
  reg [47:0] csr_epc;
  reg [47:0] csr_cause;
  reg [47:0] csr_lr;
  reg [47:0] csr_ssp;
  reg [47:0] csr_cycle;
  reg [47:0] csr_instret;
  reg [47:0] csr_timer;
  reg [47:0] csr_timer_cmp;
  reg [47:0] csr_irq_enable;
  reg [47:0] csr_irq_pending;
  reg [47:0] csr_irq_vector;

  wire status_uie  = csr_status[STATUS_UIE_BIT];
  wire status_kie  = csr_status[STATUS_KIE_BIT];
  wire status_mie  = csr_status[STATUS_MIE_BIT];
  // Exception cause codes (mirrors RISC-style numbering for familiarity)
  localparam [3:0] CAUSE_ILLEGAL_INSTR    = 4'd2;
  localparam [3:0] CAUSE_BREAKPOINT       = 4'd3;
  localparam [3:0] CAUSE_MISALIGNED_LOAD  = 4'd4;
  localparam [3:0] CAUSE_MISALIGNED_STORE = 4'd6;

  reg        exception;
  reg [3:0]  exception_code;
  reg        illegal_instr;
  reg        breakpoint_instr;
  reg        misaligned_load;
  reg        misaligned_store;
  reg        interrupt;
  reg [IRQ_INDEX_WIDTH-1:0] irq_index;
  reg        trap_taken;
  reg [47:0] trap_vector;
  reg [47:0] trap_cause_value;
  reg [47:0] trap_epc_value;
  reg        iret;
  reg        handler_active;

  reg [47:0] csr_read_value;
  reg [47:0] csr_write_value;
  reg [11:0] csr_addr_sel;
  reg        csr_write_en;
  reg        csr_illegal;
  reg [1:0]  csr_required_priv;
  reg        csr_known;
  reg        csr_valid_access;
  reg        ssp_write_en;
  reg [47:0] ssp_write_data;

  // ------------------- Regfiles -------------------
  wire [47:0] rA_pre, rD;
  wire [47:0] rA;
  wire [2:0]  a_raddr;
  reg         weA, weD;
  reg  [2:0]  wA_idx, wD_idx;
  reg  [47:0] wA_data, wD_data;

  assign a_raddr =
    (op==OP_BR)   ? br_rsA  :
    (op==OP_JALR) ? jr_rsA  :
                    rsA;

  regfileA RF_A(
    .clk(clk),
    .raddr(a_raddr),
    .rdata(rA_pre),
    .we(weA),
    .waddr(wA_idx),
    .wdata(wA_data)
  );

  assign rA =
    (handler_active && (a_raddr == 3'd7)) ? csr_ssp :
                                            rA_pre;
  wire [2:0] d_raddr =
    (op==OP_BR)     ? br_rsD  :
    (op==OP_ST)     ? st_rsD  :
    (op==OP_ALUI_D) ? rsD_im  :
                      rsD;

  regfileD RF_D(
    .clk(clk),
    .raddr(d_raddr),
    .rdata(rD),
    .we(weD),
    .waddr(wD_idx),
    .wdata(wD_data)
  );

  // ------------------- ALU -------------------
  reg  [47:0] alu_a, alu_b;
  reg  [5:0]  alu_op;
  reg  [5:0]  shamt;

  wire [47:0] alu_y;
  wire        alu_eq, alu_lt_s, alu_lt_u;

  alu48 U_ALU(
    .a_in(alu_a),
    .b_in(alu_b),
    .op(alu_op),
    .shamt(shamt),
    .y(alu_y),
    .eq(alu_eq),
    .lt_s(alu_lt_s),
    .lt_u(alu_lt_u)
  );

  // ------------------- DMEM -------------------
  wire [47:0] d_rdata;
  reg         d_we;
  reg  [$clog2(DM_WORDS)-1:0] d_addr;

  simple_mem48 #(.WORDS(DM_WORDS)) DMEM (
    .clk(clk),
    .addr(d_addr),
    .we(d_we),
    .wdata(rD),       // store writes D source
    .rdata(d_rdata)   // load reads, write back to D
  );

  wire [47:0] irq_external = {{(48-IRQ_LINES){1'b0}}, irq};
  wire        timer_irq_level = (csr_timer_cmp != 48'd0) && (csr_timer >= csr_timer_cmp);
  wire [47:0] irq_timer_mask = timer_irq_level ? TIMER_IRQ_BIT_MASK : 48'd0;
  wire [47:0] irq_signals = irq_external | irq_timer_mask;
  wire [47:0] irq_combined = csr_irq_pending | irq_signals;
  wire [47:0] irq_serviceable = irq_combined & csr_irq_enable;
  wire        irq_mode_enable =
    (priv_mode == PRIV_MACHINE)    ? status_mie :
    (priv_mode == PRIV_SUPERVISOR) ? status_kie :
                                     status_uie;

  // ------------------- Control -------------------
  reg [47:0] next_pc;
  reg        halt;

  // Main decode
  always @* begin
    integer idx;
    integer first_found;
    // defaults
    weA = 1'b0; wA_idx = 3'd0; wA_data = 48'd0;
    weD = 1'b0; wD_idx = 3'd0; wD_data = 48'd0;
    ssp_write_en = 1'b0; ssp_write_data = 48'd0;

    // default ALU wiring: use rsA, rsD; swap for commutatives when requested
    alu_a = swap ? rD : rA;
    alu_b = swap ? rA : rD;
    alu_op = 6'h00;
    shamt = 6'd0;

    // memory defaults
    d_we   = 1'b0;
    d_addr = '0;

    // next PC defaults
    next_pc = pc + 48'd1;
    halt    = 1'b0;

    // CSR defaults
    csr_read_value   = 48'd0;
    csr_write_value  = 48'd0;
    csr_addr_sel     = 12'd0;
    csr_write_en     = 1'b0;
    csr_illegal      = 1'b0;
    csr_required_priv= PRIV_MACHINE;
    csr_known        = 1'b0;
    csr_valid_access = 1'b0;
    illegal_instr    = 1'b0;
    breakpoint_instr = 1'b0;
    misaligned_load  = 1'b0;
    misaligned_store = 1'b0;
    exception        = 1'b0;
    exception_code   = 4'd0;
    interrupt        = 1'b0;
    irq_index        = {IRQ_INDEX_WIDTH{1'b0}};
    trap_taken       = 1'b0;
    trap_vector      = TRAP_VECTOR;
    trap_cause_value = 48'd0;
    trap_epc_value   = pc;
    iret             = 1'b0;
    first_found      = 0;

    for (idx = 0; idx < TOTAL_IRQ_LINES; idx = idx + 1) begin
      if (!first_found && irq_serviceable[idx]) begin
        irq_index = idx[IRQ_INDEX_WIDTH-1:0];
        first_found = 1;
      end
    end
    if (resetn && (first_found != 0) && irq_mode_enable) begin
      interrupt = 1'b1;
    end

    if (resetn) begin
      case (op)
      OP_ALU: begin
        // Map extended opcode nibble to ALU op
        reg op_valid;
        op_valid = 1'b1;
        case (op_ext)
          F_ADD: alu_op = 6'h00;
          F_SUB: alu_op = 6'h01;
          F_AND: alu_op = 6'h02;
          F_OR : alu_op = 6'h03;
          F_XOR: alu_op = 6'h04;
          F_SLL: begin alu_op = 6'h05; shamt = (swap ? rA[5:0] : rD[5:0]); end // prefer imm shifts; var allowed if using D as shamt
          F_SRL: begin alu_op = 6'h06; shamt = (swap ? rA[5:0] : rD[5:0]); end
          F_SRA: begin alu_op = 6'h07; shamt = (swap ? rA[5:0] : rD[5:0]); end
          F_NOT: alu_op = 6'h08;
          default: op_valid = 1'b0;
        endcase
        if (op_valid) begin
          if (rdBankA) begin
            weA = (rdIdx != 3'd0); wA_idx = rdIdx; wA_data = alu_y;
          end else begin
            weD = 1'b1; wD_idx = rdIdx; wD_data = alu_y;
          end
        end else begin
          illegal_instr = 1'b1;
        end
      end

      OP_ALUI_A: begin
        // Unary on A with immediate
        reg [7:0] alui_info;
        reg       op_valid;
        reg       imm_zero;
        alu_a = rA;
        alu_b = SX_imm27;  // used by ADDI; for shifts, only shamt matters
        shamt = imm27[5:0];

        alui_info = cpu_ad48_decode_alui_subop(subop[3:0]);
        op_valid  = alui_info[7];
        imm_zero  = alui_info[6];

        if (op_valid) begin
          alu_op = alui_info[5:0];
          if (imm_zero) begin
            alu_b = 48'd0;
          end
          if (rdBankA) begin
            weA = (rdIdx != 3'd0); wA_idx = rdIdx; wA_data = alu_y;
          end else begin
            weD = 1'b1; wD_idx = rdIdx; wD_data = alu_y;
          end
        end else begin
          illegal_instr = 1'b1;
        end
      end

      OP_ALUI_D: begin
        // Unary on D with immediate
        reg [7:0] alui_info;
        reg       op_valid;
        reg       imm_zero;
        alu_a = rD;
        alu_b = SX_imm27;
        shamt = imm27[5:0];

        alui_info = cpu_ad48_decode_alui_subop(subop[3:0]);
        op_valid  = alui_info[7];
        imm_zero  = alui_info[6];

        if (op_valid) begin
          alu_op = alui_info[5:0];
          if (imm_zero) begin
            alu_b = 48'd0;
          end
          if (rdBankA) begin
            weA = (rdIdx != 3'd0); wA_idx = rdIdx; wA_data = alu_y;
          end else begin
            weD = 1'b1; wD_idx = rdIdx; wD_data = alu_y;
          end
        end else begin
          illegal_instr = 1'b1;
        end
      end

      OP_LD: begin
        // address = A[baseA] + disp
        // DMEM is word-addressed: use low address bits sized to DM_WORDS
        // LD always writes to D bank (rdD)
        // Optional post-inc: A[baseA] += disp (suppressed if baseA==A0)
        if (mem_addr_invalid) begin
          misaligned_load = 1'b1;
        end else begin
          alu_a = rA; alu_b = SX_disp33; alu_op = 6'h00;
          d_addr = base_disp_idx;
          weD = 1'b1; wD_idx = ld_rdD; wD_data = d_rdata;

          // post-inc writeback to A (but never to A0)
          if (postinc && (baseA != 3'd0)) begin
            weA = 1'b1; wA_idx = baseA; wA_data = base_plus_disp;
          end
        end
      end

      OP_ST: begin
        if (mem_addr_invalid) begin
          misaligned_store = 1'b1;
        end else begin
          alu_a = rA; alu_b = SX_disp33; alu_op = 6'h00;
          d_addr = base_disp_idx;
          d_we   = 1'b1;

          if (postinc && (baseA != 3'd0)) begin
            weA = 1'b1; wA_idx = baseA; wA_data = base_plus_disp;
          end
        end
      end

      OP_BR: begin
        // Evaluate condition using comparator wires
        reg take;
        begin
          case (br_cond)
            C_BEQ:    take = (rA == rD);
            C_BNE:    take = (rA != rD);
            C_BLT:    take = ($signed(rA) <  $signed(rD));
            C_BLTU:   take = (rA < rD);
            C_BGE:    take = ($signed(rA) >= $signed(rD));
            C_BGEU:   take = (rA >= rD);
            C_ALWAYS: take = 1'b1;
            default:  take = 1'b0;
          endcase
          if (take) next_pc = pc + 48'd1 + SX_br31;
        end
      end

      OP_JAL: begin
        // link
        if (j_rdBank) begin
          weD = 1'b1; wD_idx = j_rdIdx; wD_data = pc + 48'd1;
        end else begin
          weA = (j_rdIdx != 3'd0); wA_idx = j_rdIdx; wA_data = pc + 48'd1;
        end
        // jump
        next_pc = pc + 48'd1 + SX_jal36;
      end

      OP_JALR: begin
        // target = A[jr_rsA] + imm
        if (j_rdBank) begin
          weD = 1'b1; wD_idx = j_rdIdx; wD_data = pc + 48'd1;
        end else begin
          weA = (j_rdIdx != 3'd0); wA_idx = j_rdIdx; wA_data = pc + 48'd1;
        end
        if (jalr_target_oob) begin
          illegal_instr = 1'b1;
        end else begin
          next_pc = jalr_target; // word-aligned inherently by word addressing
        end
      end

      OP_CSR: begin
        reg [CSR_META_WIDTH-1:0]          csr_meta;
        reg [CSR_META_READSEL_WIDTH-1:0]  csr_read_sel;
        reg [47:0]                        csr_write_mask;
        reg [47:0]                        csr_write_operand;
        reg                               csr_has_side_effect;
        reg                               csr_write_allowed;
        csr_addr_sel = csr_addr;

        csr_meta          = cpu_ad48_csr_lookup_meta(csr_addr);
        csr_known         = csr_meta[CSR_META_VALID_BIT];
        csr_required_priv = csr_meta[CSR_META_PRIV_MSB:CSR_META_PRIV_LSB];
        csr_read_sel      = csr_meta[CSR_META_READSEL_MSB:CSR_META_READSEL_LSB];
        csr_has_side_effect = csr_meta[CSR_META_WRITE_SIDE_BIT];
        csr_write_mask    = csr_meta[CSR_META_WRITE_MASK_MSB:CSR_META_WRITE_MASK_LSB];
        csr_write_operand = rD & csr_write_mask;
        csr_write_allowed = csr_has_side_effect || (csr_write_mask != CSR_MASK_ZERO);
        csr_read_value    = 48'd0;

        case (csr_read_sel)
          CSR_SEL_STATUS: begin
            csr_read_value = csr_status;
            csr_read_value[1:0] = priv_mode;
          end
          CSR_SEL_SCRATCH: begin
            csr_read_value = csr_scratch;
          end
          CSR_SEL_EPC: begin
            csr_read_value = csr_epc;
          end
          CSR_SEL_CAUSE: begin
            csr_read_value = csr_cause;
          end
          CSR_SEL_LR: begin
            csr_read_value = csr_lr;
          end
          CSR_SEL_SSP: begin
            csr_read_value = csr_ssp;
          end
          CSR_SEL_IRQ_ENABLE: begin
            csr_read_value = csr_irq_enable;
          end
          CSR_SEL_IRQ_PENDING: begin
            csr_read_value = csr_irq_pending;
          end
          CSR_SEL_IRQ_VECTOR: begin
            csr_read_value = csr_irq_vector;
          end
          CSR_SEL_CYCLE: begin
            csr_read_value = csr_cycle;
          end
          CSR_SEL_INSTRET: begin
            csr_read_value = csr_instret;
          end
          CSR_SEL_TIMER: begin
            csr_read_value = csr_timer;
          end
          CSR_SEL_TIMER_CMP: begin
            csr_read_value = csr_timer_cmp;
          end
          default: begin
            csr_read_value = 48'd0;
          end
        endcase

        csr_valid_access = csr_known && (priv_mode >= csr_required_priv);
        csr_illegal      = csr_known && !csr_valid_access;

        case (op_ext)
          CSR_F_RW: begin
            if (csr_valid_access) begin
              if (csr_write_allowed) begin
                csr_write_en    = 1'b1;
                csr_write_value = (csr_read_value & ~csr_write_mask) | csr_write_operand;
                csr_illegal     = 1'b0;
              end else begin
                csr_illegal     = csr_known;
              end
            end
          end
          CSR_F_RS: begin
            if (csr_valid_access && (csr_write_operand != 48'd0)) begin
              if (csr_write_allowed) begin
                csr_write_en    = 1'b1;
                csr_write_value = csr_read_value | csr_write_operand;
                csr_illegal     = 1'b0;
              end else begin
                csr_illegal     = csr_known;
              end
            end
          end
          CSR_F_RC: begin
            if (csr_valid_access && (csr_write_operand != 48'd0)) begin
              if (csr_write_allowed) begin
                csr_write_en    = 1'b1;
                csr_write_value = csr_read_value & ~csr_write_operand;
                csr_illegal     = 1'b0;
              end else begin
                csr_illegal     = csr_known;
              end
            end
          end
          CSR_F_R: begin
            if (csr_valid_access) begin
              csr_illegal = 1'b0;
            end
          end
          default: begin
            csr_illegal  = 1'b1;
            csr_write_en = 1'b0;
          end
        endcase

        if (csr_valid_access && !csr_illegal) begin
          if (rdBankA) begin
            if (rdIdx != 3'd0) begin
              weA = 1'b1; wA_idx = rdIdx; wA_data = csr_read_value;
            end
          end else begin
            weD = 1'b1; wD_idx = rdIdx; wD_data = csr_read_value;
          end
        end
        if (csr_illegal) begin
          illegal_instr = 1'b1;
        end
      end

      OP_SYS: begin
        // funct encoded in opcode lower nibble
        case (op_ext)
          4'h0: begin
            // NOP
          end
          4'h1: begin
            breakpoint_instr = 1'b1;
          end
          4'h2: begin
            if (priv_mode != PRIV_MACHINE) begin
              illegal_instr = 1'b1;
            end else begin
              next_pc = csr_lr;
              iret = 1'b1;
            end
          end
          4'hF: begin
            halt = 1'b1; next_pc = pc; // hold PC
          end
          default: begin
            illegal_instr = 1'b1;
          end
        endcase
      end

      default: illegal_instr = 1'b1;
      endcase
    end

    if (handler_active && weA) begin
      if (wA_idx == 3'd7) begin
        ssp_write_en   = 1'b1;
        ssp_write_data = wA_data;
        weA            = 1'b0;
      end
    end

    if (resetn) begin
      if (illegal_instr) begin
        exception      = 1'b1;
        exception_code = CAUSE_ILLEGAL_INSTR;
      end else if (breakpoint_instr) begin
        exception      = 1'b1;
        exception_code = CAUSE_BREAKPOINT;
      end else if (misaligned_load) begin
        exception      = 1'b1;
        exception_code = CAUSE_MISALIGNED_LOAD;
      end else if (misaligned_store) begin
        exception      = 1'b1;
        exception_code = CAUSE_MISALIGNED_STORE;
      end
    end else begin
      exception      = 1'b0;
      exception_code = 4'd0;
    end

    if (exception) begin
      trap_taken       = 1'b1;
      trap_vector      = TRAP_VECTOR;
      trap_cause_value = {44'd0, exception_code};
    end else if (interrupt) begin
      trap_taken       = 1'b1;
      trap_vector      = csr_irq_vector + {{(48-IRQ_INDEX_WIDTH){1'b0}}, irq_index};
      trap_cause_value = 48'd0;
      trap_cause_value[47] = 1'b1;
      trap_cause_value[IRQ_INDEX_WIDTH-1:0] = irq_index;
    end

    if (trap_taken) begin
      weA = 1'b0; wA_idx = 3'd0; wA_data = 48'd0;
      weD = 1'b0; wD_idx = 3'd0; wD_data = 48'd0;
      d_we = 1'b0;
      csr_write_en = 1'b0;
      ssp_write_en = 1'b0; ssp_write_data = 48'd0;
      halt = 1'b0;
      next_pc = pc;
    end
  end

  // ------------------- State update -------------------
  wire trap_pending = trap_taken;

  always @(posedge clk or negedge resetn) begin
    if (!resetn) begin
      pc                <= 48'd0;
      priv_mode         <= PRIV_MACHINE;
      csr_status        <= 48'd0;
      csr_scratch       <= 48'd0;
      csr_epc           <= 48'd0;
      csr_cause         <= 48'd0;
      csr_lr            <= 48'd0;
      csr_ssp           <= 48'd0;
      csr_cycle         <= 48'd0;
      csr_instret       <= 48'd0;
      csr_timer         <= 48'd0;
      csr_timer_cmp     <= 48'd0;
      csr_irq_enable    <= 48'd0;
      csr_irq_pending   <= 48'd0;
      csr_irq_vector    <= IRQ_VECTOR;
      handler_active    <= 1'b0;
    end else begin
      if (trap_pending) begin
        pc <= trap_vector;
      end else begin
        pc <= next_pc;
      end

      if (trap_pending) begin
        handler_active <= 1'b1;
      end else if (iret) begin
        handler_active <= 1'b0;
      end

      if (trap_pending) begin
        priv_mode <= PRIV_MACHINE;
        csr_status <= cpu_ad48_status_trap_transition(csr_status, priv_mode);
      end else if (iret) begin
        priv_mode <= cpu_ad48_status_prev_mode(csr_status);
        csr_status <= cpu_ad48_status_iret_transition(csr_status);
      end else if (csr_write_en && !csr_illegal && (csr_addr_sel == CSR_STATUS)) begin
        priv_mode <= cpu_ad48_sanitize_priv_mode(csr_write_value[1:0]);
        csr_status <= cpu_ad48_normalize_status_write(
          csr_write_value, cpu_ad48_sanitize_priv_mode(csr_write_value[1:0]));
      end else begin
        csr_status[1:0] <= priv_mode;
      end

      if (!trap_pending && csr_write_en && !csr_illegal && (csr_addr_sel == CSR_SCRATCH)) begin
        csr_scratch <= csr_write_value;
      end
      if (!trap_pending && csr_write_en && !csr_illegal && (csr_addr_sel == CSR_SSP)) begin
        csr_ssp <= csr_write_value;
      end

      if (trap_pending) begin
        csr_epc   <= trap_epc_value;
        csr_lr    <= trap_epc_value;
        csr_cause <= trap_cause_value;
      end else begin
        if (csr_write_en && !csr_illegal && (csr_addr_sel == CSR_EPC)) begin
          csr_epc <= csr_write_value;
          csr_lr  <= csr_write_value;
        end
        if (csr_write_en && !csr_illegal && (csr_addr_sel == CSR_LR)) begin
          csr_lr  <= csr_write_value;
          csr_epc <= csr_write_value;
        end
        if (csr_write_en && !csr_illegal && (csr_addr_sel == CSR_CAUSE)) begin
          csr_cause <= csr_write_value;
        end
      end

      if (csr_write_en && !csr_illegal && !trap_pending && (csr_addr_sel == CSR_CYCLE)) begin
        csr_cycle <= csr_write_value;
      end else begin
        csr_cycle <= csr_cycle + 48'd1;
      end

      if (csr_write_en && !csr_illegal && !trap_pending && (csr_addr_sel == CSR_INSTRET)) begin
        csr_instret <= csr_write_value;
      end else if (!halt && !trap_pending) begin
        csr_instret <= csr_instret + 48'd1;
      end

      if (csr_write_en && !csr_illegal && !trap_pending && (csr_addr_sel == CSR_TIMER)) begin
        csr_timer <= csr_write_value;
      end else begin
        csr_timer <= csr_timer + 48'd1;
      end

      if (csr_write_en && !csr_illegal && !trap_pending && (csr_addr_sel == CSR_TIMER_CMP)) begin
        csr_timer_cmp <= csr_write_value;
      end

      if (csr_write_en && !csr_illegal && !trap_pending && (csr_addr_sel == CSR_IRQ_ENABLE)) begin
        csr_irq_enable <= (csr_write_value & IRQ_LINE_MASK);
      end else begin
        csr_irq_enable <= (csr_irq_enable & IRQ_LINE_MASK);
      end

      if (csr_write_en && !csr_illegal && (csr_addr_sel == CSR_IRQ_VECTOR)) begin
        csr_irq_vector <= csr_write_value;
      end

      begin : pending_update
        reg [47:0] pending_new;
        pending_new = csr_irq_pending & IRQ_LINE_MASK;
        if (csr_write_en && !csr_illegal && (csr_addr_sel == CSR_IRQ_PENDING)) begin
          pending_new = csr_write_value & IRQ_LINE_MASK;
        end
        pending_new = pending_new | (irq_signals & IRQ_LINE_MASK);
        csr_irq_pending <= pending_new;
      end

      if (ssp_write_en) begin
        csr_ssp <= ssp_write_data;
      end
    end
  end

endmodule
