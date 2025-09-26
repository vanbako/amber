`timescale 1ns/1ps

module amber128_decoder
  import amber128_pkg::*;
(
    input  amber128_fetch_s     fetch_i,
    input  logic [2:0]          slot_idx_i,   // 0..4
    input  logic                sub12_i,      // 0 for first 12, 1 for second 12 when flagged
    output amber128_decode_s    decode_o,
    output logic                slot_has_two12_o
);

  amber128_decode_s d;

  // Slot mapping: flags are in [127:123], S0 payload [119:96], S1 [95:72], ... S4 [23:0]
  logic [4:0]  flags;
  logic [23:0] slot_payload;
  logic        slot_two12;

  function automatic logic [23:0] get_slot_payload(
      input logic [C_XLEN-1:0] bundle,
      input logic [2:0]        idx
  );
    int hi;
    begin
      hi = 119 - (idx * 24);
      get_slot_payload = bundle[hi -: 24];
    end
  endfunction

  always_comb begin
    logic [3:0]  opc24;
    logic [3:0]  rd4;
    logic [2:0]  cap3;
    logic [12:0] imm13;
    logic [15:0] imm16;
    logic [3:0]  rd4_next;
    logic [11:0] ins12;
    logic [3:0]  opc12;
    logic [2:0]  rd3;
    logic [2:0]  rs3;
    logic [1:0]  ins12_unused;

    opc24 = '0;
    rd4   = '0;
    cap3  = '0;
    imm13 = '0;
    imm16 = '0;
    rd4_next = '0;
    ins12 = '0;
    opc12 = '0;
    rd3   = '0;
    rs3   = '0;
    ins12_unused = '0;

    d                 = '0;
    d.valid           = fetch_i.valid;
    d.pc_word_addr    = fetch_i.word_addr;
    d.slot_idx        = slot_idx_i;
    d.sub12_idx       = sub12_i;
    d.alu_op          = ALU_PASS;
    d.branch          = BR_NONE;
    d.cap_addr_sel    = CREG_DDC;
    d.cap_data_sel    = CREG_DDC;

    flags        = fetch_i.bundle[127:123];
    slot_two12   = flags[4 - slot_idx_i];
    slot_payload = get_slot_payload(fetch_i.bundle, slot_idx_i);
    slot_has_two12_o = slot_two12;

    if (!slot_two12) begin
      // Single 24-bit instruction in this slot
      // [23:20]=opcode4, [19:16]=rd4, [15:13]=cap3, [12:0]=imm13 (in 16-byte units)
      opc24 = slot_payload[23:20];
      rd4   = slot_payload[19:16];
      cap3  = slot_payload[15:13];
      imm13 = slot_payload[12:0];

      d.rd           = rd4[DATA_REG_AW-1:0];
      d.rs           = rd4[DATA_REG_AW-1:0];
      d.imm24        = {11'b0, imm13};
      d.cap_addr_sel = cap3[CAP_REG_AW-1:0];
      d.cap_data_sel = rd4[CAP_REG_AW-1:0];

      unique case (opc24)
        4'h1: d.is_ld128 = 1'b1; // ld128 (rd, rd+1) <- [cap + imm<<4]
        4'h2: d.is_st128 = 1'b1; // st128 [cap + imm<<4] <- (rd, rd+1)
        4'h3: begin // li rd, imm16
          imm16 = {cap3, imm13};
          d.rd     = rd4[DATA_REG_AW-1:0];
          d.rs     = REG_ZERO;
          d.is_imm = 1'b1;
          d.alu_op = ALU_PASS;
          d.imm24  = {{8{imm16[15]}}, imm16};
        end
        4'h4: begin // capmov cDst, rBase
          rd4_next            = rd4 + 4'd1;
          d.cap_move          = 1'b1;
          d.cap_move_dst      = cap3[CAP_REG_AW-1:0];
          d.cap_move_src_lo   = rd4[DATA_REG_AW-1:0];
          d.cap_move_src_hi   = rd4_next[DATA_REG_AW-1:0];
          d.rd                = REG_ZERO;
          d.rs                = rd4[DATA_REG_AW-1:0];
        end
        4'h8: begin // br (unconditional) to pc + (imm << 4)
          d.branch = BR_UNCOND;
          d.rd     = REG_ZERO; // no writeback
          d.rs     = REG_ZERO;
          // Sign-extend imm13 for branches
          d.imm24  = {{11{imm13[12]}}, imm13};
        end
        4'h9: begin // beq rd4, rs3, +imm13
          d.branch = BR_EQ;
          d.rd     = rd4[DATA_REG_AW-1:0];
          d.rs     = {1'b0, cap3}[DATA_REG_AW-1:0];
          d.imm24  = {{11{imm13[12]}}, imm13};
        end
        4'hA: begin // bne rd4, rs3, +imm13
          d.branch = BR_NE;
          d.rd     = rd4[DATA_REG_AW-1:0];
          d.rs     = {1'b0, cap3}[DATA_REG_AW-1:0];
          d.imm24  = {{11{imm13[12]}}, imm13};
        end
        default: begin
          d.valid = fetch_i.valid; // will be treated as illegal upstream if needed
          d.rd    = '0;
        end
      endcase
    end else begin
      // Two 12-bit instructions packed in this slot
      // Order: [23:12]=first, [11:0]=second
      ins12 = sub12_i ? slot_payload[11:0] : slot_payload[23:12];
      // [11:8]=opcode4, [7:5]=rd3, [4:2]=rs3, [1:0] ignored (can be used later)
      opc12 = ins12[11:8];
      rd3   = ins12[7:5];
      rs3   = ins12[4:2];
      ins12_unused = ins12[1:0];
      d.rd  = {1'b0, rd3}[DATA_REG_AW-1:0];
      d.rs  = {1'b0, rs3}[DATA_REG_AW-1:0];

      unique case (opc12)
        4'h0: begin // nop
          d.alu_op = ALU_PASS;
          d.rd     = REG_ZERO;
        end
        4'h1: d.alu_op = ALU_ADD;
        4'h2: d.alu_op = ALU_SUB;
        4'h3: d.alu_op = ALU_AND;
        4'h4: d.alu_op = ALU_OR;
        4'h5: d.alu_op = ALU_XOR;
        4'h6: d.alu_op = ALU_LSL;
        4'h7: d.alu_op = ALU_LSR;
        4'h8: d.alu_op = ALU_PASS; // mov rd, rs
        default: begin
          d.alu_op = ALU_PASS;
          d.rd     = REG_ZERO;
        end
      endcase
    end
  end

  assign decode_o = d;

endmodule
