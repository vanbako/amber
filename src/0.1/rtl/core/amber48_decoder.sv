module amber48_decoder
  import amber48_pkg::*;
(
    input  amber48_decode_in_s  fetch_i,
    output amber48_decode_out_s decode_o
);

  localparam logic [7:0] OPCODE_UPPER_IMM      = 8'h00;
  localparam logic [7:0] OPCODE_ADD_REG        = 8'h10;
  localparam logic [7:0] OPCODE_ADD_IMM        = 8'h11;
  localparam logic [7:0] OPCODE_SUB_REG        = 8'h12;
  localparam logic [7:0] OPCODE_SUB_IMM        = 8'h13;
  localparam logic [7:0] OPCODE_AND_REG        = 8'h20;
  localparam logic [7:0] OPCODE_OR_REG         = 8'h21;
  localparam logic [7:0] OPCODE_XOR_REG        = 8'h22;
  localparam logic [7:0] OPCODE_XOR_IMM        = 8'h23;
  localparam logic [7:0] OPCODE_LSL            = 8'h30;
  localparam logic [7:0] OPCODE_LSR            = 8'h31;
  localparam logic [7:0] OPCODE_BRANCH_EQ      = 8'h40;
  localparam logic [7:0] OPCODE_BRANCH_NE      = 8'h41;
  localparam logic [7:0] OPCODE_BRANCH_LTU     = 8'h42;
  localparam logic [7:0] OPCODE_BRANCH_LTS     = 8'h43;
  localparam logic [7:0] OPCODE_BRANCH_GTU     = 8'h44;
  localparam logic [7:0] OPCODE_BRANCH_GTS     = 8'h45;
  localparam logic [7:0] OPCODE_BRANCH_ZERO    = 8'h46;
  localparam logic [7:0] OPCODE_BRANCH_NOTZERO = 8'h47;
  localparam logic [7:0] OPCODE_BRANCH_ALWAYS  = 8'h48;
  localparam logic [7:0] OPCODE_LOAD           = 8'h60;
  localparam logic [7:0] OPCODE_STORE          = 8'h61;
  localparam logic [7:0] OPCODE_JUMP           = 8'h70;
  localparam logic [7:0] OPCODE_JUMP_SUB       = 8'h71;
  localparam logic [7:0] OPCODE_RETURN         = 8'h72;

  amber48_decode_out_s              decode_r;
  logic [7:0]                       opcode;
  logic [REG_ADDR_WIDTH-1:0]        rd_field;
  logic [REG_ADDR_WIDTH-1:0]        rs1_field;
  logic [REG_ADDR_WIDTH-1:0]        rs2_field;
  logic [XLEN-1:0]                  imm_ext;
  logic [23:0]                      upper_imm_field;
  logic [XLEN-1:0]                  upper_imm_value;

  always_comb begin
    decode_r            = '0;
    opcode              = fetch_i.instr[47:40];
    rd_field            = fetch_i.instr[15:12];
    rs1_field           = fetch_i.instr[23:20];
    rs2_field           = fetch_i.instr[19:16];
    imm_ext             = {{(XLEN-16){fetch_i.instr[39]}}, fetch_i.instr[39:24]};
    upper_imm_field    = fetch_i.instr[39:16];
    upper_imm_value    = {upper_imm_field, 24'b0};

    decode_r.valid      = fetch_i.valid;
    decode_r.pc         = fetch_i.pc;
    decode_r.rs1        = rs1_field;
    decode_r.rs2        = rs2_field;
    decode_r.rd         = rd_field;
    decode_r.alu_op     = ALU_PASS;
    decode_r.imm        = imm_ext;
    decode_r.uses_imm   = 1'b0;
    decode_r.branch_type= BR_NONE;
    decode_r.load       = 1'b0;
    decode_r.store      = 1'b0;
    decode_r.trap       = 1'b0;
    decode_r.trap_cause = TRAP_NONE;

    case (opcode)
      OPCODE_UPPER_IMM: begin
        decode_r.alu_op   = ALU_PASS;
        decode_r.uses_imm = 1'b1;
        decode_r.rs1      = REG_ZERO;
        decode_r.rs2      = REG_ZERO;
        decode_r.imm      = upper_imm_value;
      end
      OPCODE_ADD_REG: begin
        decode_r.alu_op = ALU_ADD;
      end
      OPCODE_ADD_IMM: begin
        decode_r.alu_op   = ALU_ADD;
        decode_r.uses_imm = 1'b1;
      end
      OPCODE_SUB_REG: begin
        decode_r.alu_op = ALU_SUB;
      end
      OPCODE_SUB_IMM: begin
        decode_r.alu_op   = ALU_SUB;
        decode_r.uses_imm = 1'b1;
      end
      OPCODE_AND_REG: begin
        decode_r.alu_op = ALU_AND;
      end
      OPCODE_OR_REG: begin
        decode_r.alu_op = ALU_OR;
      end
      OPCODE_XOR_REG: begin
        decode_r.alu_op = ALU_XOR;
      end
      OPCODE_XOR_IMM: begin
        decode_r.alu_op   = ALU_XOR;
        decode_r.uses_imm = 1'b1;
      end
      OPCODE_LSL: begin
        decode_r.alu_op = ALU_LSL;
      end
      OPCODE_LSR: begin
        decode_r.alu_op = ALU_LSR;
      end
      OPCODE_BRANCH_EQ: begin
        decode_r.branch_type = BR_EQ;
        decode_r.rd          = '0;
      end
      OPCODE_BRANCH_NE: begin
        decode_r.branch_type = BR_NE;
        decode_r.rd          = '0;
      end
      OPCODE_BRANCH_LTU: begin
        decode_r.branch_type = BR_LT_U;
        decode_r.rd          = '0;
      end
      OPCODE_BRANCH_LTS: begin
        decode_r.branch_type = BR_LT_S;
        decode_r.rd          = '0;
      end
      OPCODE_BRANCH_GTU: begin
        decode_r.branch_type = BR_GT_U;
        decode_r.rd          = '0;
      end
      OPCODE_BRANCH_GTS: begin
        decode_r.branch_type = BR_GT_S;
        decode_r.rd          = '0;
      end
      OPCODE_BRANCH_ZERO: begin
        decode_r.branch_type = BR_ZERO;
        decode_r.rd          = '0;
        decode_r.rs2         = '0;
      end
      OPCODE_BRANCH_NOTZERO: begin
        decode_r.branch_type = BR_NOT_ZERO;
        decode_r.rd          = '0;
        decode_r.rs2         = '0;
      end
      OPCODE_BRANCH_ALWAYS: begin
        decode_r.branch_type = BR_UNCOND;
        decode_r.rd          = '0;
        decode_r.rs2         = '0;
      end
      OPCODE_JUMP: begin
        decode_r.branch_type = BR_UNCOND;
        decode_r.rs1         = REG_ZERO;
        decode_r.rs2         = REG_ZERO;
        decode_r.rd          = REG_ZERO;
        decode_r.uses_imm    = 1'b1;
        decode_r.imm         = imm_ext;
        decode_r.is_jump     = 1'b1;
      end
      OPCODE_JUMP_SUB: begin
        decode_r.branch_type = BR_UNCOND;
        decode_r.rs1         = REG_ZERO;
        decode_r.rs2         = REG_ZERO;
        decode_r.rd          = REG_LR;
        decode_r.uses_imm    = 1'b1;
        decode_r.imm         = imm_ext;
        decode_r.is_jump_sub = 1'b1;
      end
      OPCODE_RETURN: begin
        decode_r.branch_type = BR_UNCOND;
        decode_r.rs1         = (rs1_field == REG_ZERO) ? REG_LR : rs1_field;
        decode_r.rs2         = REG_ZERO;
        decode_r.rd          = REG_ZERO;
        decode_r.uses_imm    = 1'b0;
        decode_r.imm         = '0;
        decode_r.is_return   = 1'b1;
      end
      OPCODE_LOAD: begin
        decode_r.load      = 1'b1;
        decode_r.uses_imm  = 1'b1;
      end
      OPCODE_STORE: begin
        decode_r.store     = 1'b1;
        decode_r.uses_imm  = 1'b1;
        decode_r.rd        = '0;
      end
      default: begin
        if (fetch_i.valid) begin
          decode_r.trap       = 1'b1;
          decode_r.trap_cause = TRAP_ILLEGAL;
          decode_r.rd         = '0;
        end
      end
    endcase
  end

  assign decode_o = decode_r;

endmodule



