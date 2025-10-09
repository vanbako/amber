`ifndef CPU_AD48_INSTR_VH
`define CPU_AD48_INSTR_VH

// Opcode map
localparam [7:0] OP_ALU    = 8'h00;
localparam [7:0] OP_ALUI_A = 8'h01;
localparam [7:0] OP_ALUI_D = 8'h02;
localparam [7:0] OP_LD     = 8'h03;
localparam [7:0] OP_ST     = 8'h04;
localparam [7:0] OP_BR     = 8'h05;
localparam [7:0] OP_JAL    = 8'h06;
localparam [7:0] OP_JALR   = 8'h07;
localparam [7:0] OP_SYS    = 8'h3F;

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

function [47:0] instr_alu;
  input         rdBank; // 0 = A, 1 = D
  input  [2:0]  rdIdx;
  input  [2:0]  rsA;
  input  [2:0]  rsD;
  input  [3:0]  funct;
  input         swap;
  begin
    instr_alu = {OP_ALU, rdBank, rdIdx, rsA, rsD, funct, swap, 25'd0};
  end
endfunction

function [47:0] instr_alui_a;
  input         rdBank;
  input  [2:0]  rdIdx;
  input  [2:0]  rsA;
  input  [5:0]  subop;
  input  [26:0] imm27;
  begin
    instr_alui_a = {OP_ALUI_A, rdBank, rdIdx, rsA, subop, imm27};
  end
endfunction

function [47:0] instr_alui_d;
  input         rdBank;
  input  [2:0]  rdIdx;
  input  [2:0]  rsD;
  input  [5:0]  subop;
  input  [26:0] imm27;
  begin
    instr_alui_d = {OP_ALUI_D, rdBank, rdIdx, rsD, subop, imm27};
  end
endfunction

function [47:0] instr_ld;
  input         postinc;
  input  [2:0]  rdD;
  input  [2:0]  baseA;
  input  [32:0] disp33;
  begin
    instr_ld = {OP_LD, postinc, rdD, baseA, disp33};
  end
endfunction

function [47:0] instr_st;
  input         postinc;
  input  [2:0]  rsD;
  input  [2:0]  baseA;
  input  [32:0] disp33;
  begin
    instr_st = {OP_ST, postinc, rsD, baseA, disp33};
  end
endfunction

function [47:0] instr_br;
  input  [2:0]  cond;
  input  [2:0]  rsA;
  input  [2:0]  rsD;
  input  [30:0] off31;
  begin
    instr_br = {OP_BR, cond, rsA, rsD, off31};
  end
endfunction

function [47:0] instr_jal;
  input         rdBank;
  input  [2:0]  rdIdx;
  input  [35:0] off36;
  begin
    instr_jal = {OP_JAL, rdBank, rdIdx, off36};
  end
endfunction

function [47:0] instr_jalr;
  input         rdBank;
  input  [2:0]  rdIdx;
  input  [2:0]  rsA;
  input  [32:0] imm33;
  begin
    instr_jalr = {OP_JALR, rdBank, rdIdx, rsA, imm33};
  end
endfunction

function [47:0] instr_sys;
  input  [3:0]  funct;
  begin
    instr_sys = {OP_SYS, funct, 36'd0};
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

function [47:0] to48;
  input signed [63:0] value;
  begin
    to48 = value[47:0];
  end
endfunction

`endif // CPU_AD48_INSTR_VH
