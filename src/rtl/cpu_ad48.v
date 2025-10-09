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
  parameter [47:0] TRAP_VECTOR = 48'd64
)(
  input  clk,
  input  resetn
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

  // Sign-extends
  wire [47:0] SX_imm27  = {{21{imm27[26]}}, imm27};
  wire [47:0] SX_disp33 = {{15{disp33[32]}}, disp33};
  wire [47:0] SX_br31   = {{17{br_off31[30]}}, br_off31};
  wire [47:0] SX_jal36  = {{12{jal_off[35]}}, jal_off};
  wire [47:0] SX_jr33   = {{15{jr_imm33[32]}}, jr_imm33};
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

  // ------------------- CSR constants & state -------------------
  localparam [1:0] PRIV_USER       = 2'd0;
  localparam [1:0] PRIV_SUPERVISOR = 2'd1;
  localparam [1:0] PRIV_MACHINE    = 2'd3;

  localparam [11:0] CSR_STATUS     = 12'h000;
  localparam [11:0] CSR_SCRATCH    = 12'h001;
  localparam [11:0] CSR_EPC        = 12'h002;
  localparam [11:0] CSR_CAUSE      = 12'h003;
  localparam [11:0] CSR_CYCLE      = 12'hC00;
  localparam [11:0] CSR_INSTRET    = 12'hC01;

  reg [1:0]  priv_mode;
  reg [47:0] csr_status;
  reg [47:0] csr_scratch;
  reg [47:0] csr_epc;
  reg [47:0] csr_cause;
  reg [47:0] csr_cycle;
  reg [47:0] csr_instret;

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

  reg [47:0] csr_read_value;
  reg [47:0] csr_write_value;
  reg [11:0] csr_addr_sel;
  reg        csr_write_en;
  reg        csr_illegal;
  reg [1:0]  csr_required_priv;
  reg        csr_known;
  reg        csr_valid_access;

  // ------------------- Regfiles -------------------
  wire [47:0] rA, rD;
  reg         weA, weD;
  reg  [2:0]  wA_idx, wD_idx;
  reg  [47:0] wA_data, wD_data;

  regfileA RF_A(
    .clk(clk),
    .raddr((op==OP_BR) ? br_rsA : (op==OP_JALR ? jr_rsA : rsA)),
    .rdata(rA),
    .we(weA),
    .waddr(wA_idx),
    .wdata(wA_data)
  );
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

  // ------------------- Control -------------------
  reg [47:0] next_pc;
  reg        halt;

  // Main decode
  always @* begin
    // defaults
    weA = 1'b0; wA_idx = 3'd0; wA_data = 48'd0;
    weD = 1'b0; wD_idx = 3'd0; wD_data = 48'd0;

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
      alu_a = rA;
      alu_b = SX_imm27;  // used by ADDI; for shifts, only shamt matters
      shamt = imm27[5:0];

        begin
          reg op_valid;
          op_valid = 1'b1;
          case (subop[3:0])
            F_ADD: alu_op = 6'h00; // ADDI_A
            F_AND: alu_op = 6'h02; // ANDI_A
            F_OR : alu_op = 6'h03; // ORI_A
            F_XOR: alu_op = 6'h04; // XORI_A
            F_SLL: alu_op = 6'h05; // SLLI_A
            F_SRL: alu_op = 6'h06; // SRLI_A
            F_SRA: alu_op = 6'h07; // SRAI_A
            F_NOT: begin alu_op = 6'h08; alu_b = 48'd0; end // NOT A
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
      end

      OP_ALUI_D: begin
        // Unary on D with immediate
      alu_a = rD;
      alu_b = SX_imm27;
      shamt = imm27[5:0];

        begin
          reg op_valid;
          op_valid = 1'b1;
          case (subop[3:0])
            F_ADD: alu_op = 6'h00; // ADDI_D
            F_AND: alu_op = 6'h02; // ANDI_D
            F_OR : alu_op = 6'h03; // ORI_D
            F_XOR: alu_op = 6'h04; // XORI_D
            F_SLL: alu_op = 6'h05; // SLLI_D
            F_SRL: alu_op = 6'h06; // SRLI_D
            F_SRA: alu_op = 6'h07; // SRAI_D
            F_NOT: begin alu_op = 6'h08; alu_b = 48'd0; end // NOT D
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
        csr_addr_sel      = csr_addr;
        csr_required_priv = PRIV_MACHINE;
        csr_known         = 1'b0;

        case (csr_addr)
          CSR_STATUS: begin
            csr_read_value   = {csr_status[47:2], priv_mode};
            csr_required_priv= PRIV_MACHINE;
            csr_known        = 1'b1;
          end
          CSR_SCRATCH: begin
            csr_read_value   = csr_scratch;
            csr_required_priv= PRIV_MACHINE;
            csr_known        = 1'b1;
          end
          CSR_EPC: begin
            csr_read_value   = csr_epc;
            csr_required_priv= PRIV_MACHINE;
            csr_known        = 1'b1;
          end
          CSR_CAUSE: begin
            csr_read_value   = csr_cause;
            csr_required_priv= PRIV_MACHINE;
            csr_known        = 1'b1;
          end
          CSR_CYCLE: begin
            csr_read_value   = csr_cycle;
            csr_required_priv= PRIV_USER;
            csr_known        = 1'b1;
          end
          CSR_INSTRET: begin
            csr_read_value   = csr_instret;
            csr_required_priv= PRIV_USER;
            csr_known        = 1'b1;
          end
          default: begin
            csr_read_value = 48'd0;
            csr_known      = 1'b0;
          end
        endcase

        csr_valid_access = csr_known && (priv_mode >= csr_required_priv);
        csr_illegal      = ~csr_valid_access;

        case (op_ext)
          CSR_F_RW: begin
            if (csr_valid_access) begin
              csr_write_en    = 1'b1;
              csr_write_value = rD;
              csr_illegal     = 1'b0;
            end
          end
          CSR_F_RS: begin
            if (csr_valid_access && (rD != 48'd0)) begin
              csr_write_en    = 1'b1;
              csr_write_value = csr_read_value | rD;
              csr_illegal     = 1'b0;
            end
          end
          CSR_F_RC: begin
            if (csr_valid_access && (rD != 48'd0)) begin
              csr_write_en    = 1'b1;
              csr_write_value = csr_read_value & ~rD;
              csr_illegal     = 1'b0;
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
      weA = 1'b0; wA_idx = 3'd0; wA_data = 48'd0;
      weD = 1'b0; wD_idx = 3'd0; wD_data = 48'd0;
      d_we = 1'b0;
      csr_write_en = 1'b0;
      halt = 1'b0;
      next_pc = pc;
    end
  end

  // ------------------- State update -------------------
  wire exception_pending = exception;

  always @(posedge clk or negedge resetn) begin
    if (!resetn) begin
      pc <= 48'd0;
      priv_mode   <= PRIV_MACHINE;
      csr_status  <= 48'd0;
      csr_scratch <= 48'd0;
      csr_epc     <= 48'd0;
      csr_cause   <= 48'd0;
      csr_cycle   <= 48'd0;
      csr_instret <= 48'd0;
    end else begin
      if (exception_pending) begin
        pc <= TRAP_VECTOR;
      end else begin
        pc <= next_pc;
      end

      if (exception_pending) begin
        priv_mode  <= PRIV_MACHINE;
        csr_status <= {csr_status[47:2], PRIV_MACHINE};
      end else if (csr_write_en && !csr_illegal && (csr_addr_sel == CSR_STATUS)) begin
        priv_mode  <= csr_write_value[1:0];
        csr_status <= {csr_write_value[47:2], csr_write_value[1:0]};
      end else begin
        csr_status[1:0] <= priv_mode;
      end

      if (!exception_pending && csr_write_en && !csr_illegal && (csr_addr_sel == CSR_SCRATCH)) begin
        csr_scratch <= csr_write_value;
      end

      if (exception_pending) begin
        csr_epc   <= pc;
        csr_cause <= {44'd0, exception_code};
      end else begin
        if (csr_write_en && !csr_illegal && (csr_addr_sel == CSR_EPC)) begin
          csr_epc <= csr_write_value;
        end
        if (csr_write_en && !csr_illegal && (csr_addr_sel == CSR_CAUSE)) begin
          csr_cause <= csr_write_value;
        end
      end

      if (csr_write_en && !csr_illegal && !exception_pending && (csr_addr_sel == CSR_CYCLE)) begin
        csr_cycle <= csr_write_value;
      end else begin
        csr_cycle <= csr_cycle + 48'd1;
      end

      if (csr_write_en && !csr_illegal && !exception_pending && (csr_addr_sel == CSR_INSTRET)) begin
        csr_instret <= csr_write_value;
      end else if (!halt && !exception_pending) begin
        csr_instret <= csr_instret + 48'd1;
      end
    end
  end
endmodule
