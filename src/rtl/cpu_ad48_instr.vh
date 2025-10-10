`ifndef CPU_AD48_INSTR_VH
`define CPU_AD48_INSTR_VH

// Opcode map (upper nibble selects the instruction class, lower nibble extends the opcode)
localparam [3:0] OP_ALU    = 4'h0;
localparam [3:0] OP_ALUI_A = 4'h1;
localparam [3:0] OP_ALUI_D = 4'h2;
localparam [3:0] OP_LD     = 4'h3;
localparam [3:0] OP_ST     = 4'h4;
localparam [3:0] OP_BR     = 4'h5;
localparam [3:0] OP_JAL    = 4'h6;
localparam [3:0] OP_JALR   = 4'h7;
localparam [3:0] OP_CSR    = 4'h8;
localparam [3:0] OP_SYS    = 4'hF;
localparam [3:0] SYS_F_NOP   = 4'h0;
localparam [3:0] SYS_F_BREAK = 4'h1;
localparam [3:0] SYS_F_IRET  = 4'h2;
localparam [3:0] SYS_F_HALT  = 4'hF;

// ALU funct / subop encodings
localparam [3:0] F_ADD = 4'h0;
localparam [3:0] F_SUB = 4'h1;
localparam [3:0] F_AND = 4'h2;
localparam [3:0] F_OR  = 4'h3;
localparam [3:0] F_XOR = 4'h4;
localparam [3:0] F_SLL = 4'h5;
localparam [3:0] F_SRL = 4'h6;
localparam [3:0] F_SRA = 4'h7;
localparam [3:0] F_NOT = 4'h8;

// Branch condition encodings
localparam [2:0] C_BEQ   = 3'b000;
localparam [2:0] C_BNE   = 3'b001;
localparam [2:0] C_BLT   = 3'b010;
localparam [2:0] C_BLTU  = 3'b011;
localparam [2:0] C_BGE   = 3'b100;
localparam [2:0] C_BGEU  = 3'b101;
localparam [2:0] C_ALWAYS= 3'b111;

// CSR funct encodings
localparam [3:0] CSR_F_RW  = 4'h0; // write, return old value
localparam [3:0] CSR_F_RS  = 4'h1; // set bits, return old value
localparam [3:0] CSR_F_RC  = 4'h2; // clear bits, return old value
localparam [3:0] CSR_F_R   = 4'h3; // read only
localparam [11:0] CSR_ADDR_STATUS      = 12'h000;
localparam [11:0] CSR_ADDR_SCRATCH     = 12'h001;
localparam [11:0] CSR_ADDR_EPC         = 12'h002;
localparam [11:0] CSR_ADDR_CAUSE       = 12'h003;
localparam [11:0] CSR_ADDR_LR          = 12'h004;
localparam [11:0] CSR_ADDR_SSP         = 12'h005;
localparam [11:0] CSR_ADDR_IRQ_ENABLE  = 12'h010;
localparam [11:0] CSR_ADDR_IRQ_PENDING = 12'h011;
localparam [11:0] CSR_ADDR_IRQ_VECTOR  = 12'h012;
localparam [11:0] CSR_ADDR_CYCLE       = 12'hC00;
localparam [11:0] CSR_ADDR_INSTRET     = 12'hC01;
localparam [11:0] CSR_ADDR_TIMER       = 12'hC02;
localparam [11:0] CSR_ADDR_TIMER_CMP   = 12'hC03;

function [47:0] instr_alu;
  input         rdBank; // 0 = A, 1 = D
  input  [2:0]  rdIdx;
  input  [2:0]  rsA;
  input  [2:0]  rsD;
  input  [3:0]  funct;
  input         swap;
  begin
    // Pack the frequently-used ALU format into the upper 24 bits to allow 24-bit slots later on.
    instr_alu = {OP_ALU, funct, rdBank, rdIdx, rsA, rsD, swap, 5'd0, 24'd0};
  end
endfunction

function [47:0] instr_alui_a;
  input         rdBank;
  input  [2:0]  rdIdx;
  input  [2:0]  rsA;
  input  [5:0]  subop;
  input  [26:0] imm27;
  begin
    instr_alui_a = {OP_ALUI_A, 4'd0, rdBank, rdIdx, rsA, subop, imm27};
  end
endfunction

function [47:0] instr_alui_d;
  input         rdBank;
  input  [2:0]  rdIdx;
  input  [2:0]  rsD;
  input  [5:0]  subop;
  input  [26:0] imm27;
  begin
    instr_alui_d = {OP_ALUI_D, 4'd0, rdBank, rdIdx, rsD, subop, imm27};
  end
endfunction

function [47:0] instr_ld;
  input         postinc;
  input  [2:0]  rdD;
  input  [2:0]  baseA;
  input  [32:0] disp33;
  begin
    instr_ld = {OP_LD, 4'd0, postinc, rdD, baseA, disp33};
  end
endfunction

function [47:0] instr_st;
  input         postinc;
  input  [2:0]  rsD;
  input  [2:0]  baseA;
  input  [32:0] disp33;
  begin
    instr_st = {OP_ST, 4'd0, postinc, rsD, baseA, disp33};
  end
endfunction

function [47:0] instr_br;
  input  [2:0]  cond;
  input  [2:0]  rsA;
  input  [2:0]  rsD;
  input  [30:0] off31;
  begin
    instr_br = {OP_BR, 4'd0, cond, rsA, rsD, off31};
  end
endfunction

function [47:0] instr_jal;
  input         rdBank;
  input  [2:0]  rdIdx;
  input  [35:0] off36;
  begin
    instr_jal = {OP_JAL, 4'd0, rdBank, rdIdx, off36};
  end
endfunction

function [47:0] instr_jalr;
  input         rdBank;
  input  [2:0]  rdIdx;
  input  [2:0]  rsA;
  input  [32:0] imm33;
  begin
    instr_jalr = {OP_JALR, 4'd0, rdBank, rdIdx, rsA, imm33};
  end
endfunction

function [47:0] instr_sys;
  input  [3:0]  funct;
  begin
    instr_sys = {OP_SYS, funct, 40'd0};
  end
endfunction

function [47:0] instr_csr;
  input  [3:0]  funct;
  input         rdBank;
  input  [2:0]  rdIdx;
  input  [2:0]  rsD;
  input  [11:0] csr_addr;
  begin
    instr_csr = {OP_CSR, funct, rdBank, rdIdx, 3'd0, rsD, 1'b0, 5'd0, csr_addr, 12'd0};
  end
endfunction

function [5:0] pack_subop;
  input [3:0] funct;
  begin
    pack_subop = {2'b00, funct};
  end
endfunction

function [26:0] pack_imm27;
  input signed [47:0] value;
  begin
    pack_imm27 = value[26:0];
  end
endfunction

function [32:0] pack_disp33;
  input signed [47:0] value;
  begin
    pack_disp33 = value[32:0];
  end
endfunction

function [30:0] pack_off31;
  input signed [47:0] value;
  begin
    pack_off31 = value[30:0];
  end
endfunction

function [35:0] pack_off36;
  input signed [47:0] value;
  begin
    pack_off36 = value[35:0];
  end
endfunction

function [32:0] pack_imm33;
  input signed [47:0] value;
  begin
    pack_imm33 = value[32:0];
  end
endfunction

function [11:0] pack_csr_addr;
  input [15:0] value;
  begin
    pack_csr_addr = value[11:0];
  end
endfunction

function [47:0] to48;
  input signed [63:0] value;
  begin
    to48 = value[47:0];
  end
endfunction

`endif // CPU_AD48_INSTR_VH
