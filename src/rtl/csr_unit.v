`timescale 1ns/1ps

module csr_unit #(
  parameter integer CSR_REQ_WIDTH_P  = 71,
  parameter integer CSR_RESP_WIDTH_P = 99,
  parameter integer CSR_WB_WIDTH_P   = 53,
  parameter [47:0]  IRQ_LINE_MASK    = 48'd0
)(
  input  [CSR_REQ_WIDTH_P-1:0]  req,
  input  [47:0]                 csr_status,
  input  [47:0]                 csr_scratch,
  input  [47:0]                 csr_epc,
  input  [47:0]                 csr_cause,
  input  [47:0]                 csr_lr,
  input  [47:0]                 csr_ssp,
  input  [47:0]                 csr_irq_enable,
  input  [47:0]                 csr_irq_pending,
  input  [47:0]                 csr_irq_vector,
  input  [47:0]                 csr_cycle,
  input  [47:0]                 csr_instret,
  input  [47:0]                 csr_timer,
  input  [47:0]                 csr_timer_cmp,
  output [CSR_RESP_WIDTH_P-1:0] resp,
  output [CSR_WB_WIDTH_P-1:0]   writeback
);
  `ifdef CPU_AD48_CSR_IF_VH
    `undef CPU_AD48_CSR_IF_VH
  `endif
  `include "cpu_ad48_csr_if.vh"

  initial begin
    if (CSR_REQ_WIDTH_P  != CSR_REQ_WIDTH)  $error("csr_unit: CSR_REQ_WIDTH mismatch");
    if (CSR_RESP_WIDTH_P != CSR_RESP_WIDTH) $error("csr_unit: CSR_RESP_WIDTH mismatch");
    if (CSR_WB_WIDTH_P   != CSR_WB_WIDTH)   $error("csr_unit: CSR_WB_WIDTH mismatch");
  end
  localparam [1:0] PRIV_USER       = 2'd0;
  localparam [1:0] PRIV_SUPERVISOR = 2'd1;
  localparam [1:0] PRIV_MACHINE    = 2'd3;

  localparam [3:0] CSR_F_RW = 4'h0;
  localparam [3:0] CSR_F_RS = 4'h1;
  localparam [3:0] CSR_F_RC = 4'h2;
  localparam [3:0] CSR_F_R  = 4'h3;

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

  localparam [47:0] CSR_MASK_ZERO   = 48'd0;
  localparam [47:0] CSR_MASK_FULL   = {48{1'b1}};
  localparam [47:0] CSR_MASK_STATUS = CSR_MASK_FULL;

  localparam integer CSR_META_PRIV_WIDTH       = 2;
  localparam integer CSR_META_READSEL_WIDTH    = 4;
  localparam integer CSR_META_WRITE_MASK_WIDTH = 48;
  localparam integer CSR_META_WIDTH =
    1 + CSR_META_PRIV_WIDTH + CSR_META_READSEL_WIDTH + 1 + CSR_META_WRITE_MASK_WIDTH;
  localparam integer CSR_META_WRITE_MASK_LSB = 0;
  localparam integer CSR_META_WRITE_MASK_MSB =
    CSR_META_WRITE_MASK_LSB + CSR_META_WRITE_MASK_WIDTH - 1;
  localparam integer CSR_META_WRITE_SIDE_BIT = CSR_META_WRITE_MASK_MSB + 1;
  localparam integer CSR_META_READSEL_LSB    = CSR_META_WRITE_SIDE_BIT + 1;
  localparam integer CSR_META_READSEL_MSB    =
    CSR_META_READSEL_LSB + CSR_META_READSEL_WIDTH - 1;
  localparam integer CSR_META_PRIV_LSB       = CSR_META_READSEL_MSB + 1;
  localparam integer CSR_META_PRIV_MSB       =
    CSR_META_PRIV_LSB + CSR_META_PRIV_WIDTH - 1;
  localparam integer CSR_META_VALID_BIT      = CSR_META_PRIV_MSB + 1;

  function automatic [CSR_META_WIDTH-1:0] csr_lookup_meta;
    input [11:0] addr;
    begin
      case (addr)
        CSR_STATUS:      csr_lookup_meta = {1'b1, PRIV_SUPERVISOR, CSR_SEL_STATUS,      1'b1, CSR_MASK_STATUS};
        CSR_SCRATCH:     csr_lookup_meta = {1'b1, PRIV_SUPERVISOR, CSR_SEL_SCRATCH,     1'b0, CSR_MASK_FULL};
        CSR_EPC:         csr_lookup_meta = {1'b1, PRIV_SUPERVISOR, CSR_SEL_EPC,         1'b1, CSR_MASK_FULL};
        CSR_CAUSE:       csr_lookup_meta = {1'b1, PRIV_SUPERVISOR, CSR_SEL_CAUSE,       1'b0, CSR_MASK_FULL};
        CSR_LR:          csr_lookup_meta = {1'b1, PRIV_SUPERVISOR, CSR_SEL_LR,          1'b1, CSR_MASK_FULL};
        CSR_SSP:         csr_lookup_meta = {1'b1, PRIV_SUPERVISOR, CSR_SEL_SSP,         1'b0, CSR_MASK_FULL};
        CSR_IRQ_ENABLE:  csr_lookup_meta = {1'b1, PRIV_SUPERVISOR, CSR_SEL_IRQ_ENABLE,  1'b0, IRQ_LINE_MASK};
        CSR_IRQ_PENDING: csr_lookup_meta = {1'b1, PRIV_SUPERVISOR, CSR_SEL_IRQ_PENDING, 1'b1, IRQ_LINE_MASK};
        CSR_IRQ_VECTOR:  csr_lookup_meta = {1'b1, PRIV_SUPERVISOR, CSR_SEL_IRQ_VECTOR,  1'b0, CSR_MASK_FULL};
        CSR_CYCLE:       csr_lookup_meta = {1'b1, PRIV_USER,       CSR_SEL_CYCLE,       1'b0, CSR_MASK_FULL};
        CSR_INSTRET:     csr_lookup_meta = {1'b1, PRIV_USER,       CSR_SEL_INSTRET,     1'b0, CSR_MASK_FULL};
        CSR_TIMER:       csr_lookup_meta = {1'b1, PRIV_SUPERVISOR, CSR_SEL_TIMER,       1'b0, CSR_MASK_FULL};
        CSR_TIMER_CMP:   csr_lookup_meta = {1'b1, PRIV_SUPERVISOR, CSR_SEL_TIMER_CMP,   1'b0, CSR_MASK_FULL};
        default:         csr_lookup_meta = {1'b0, PRIV_MACHINE,    CSR_SEL_ZERO,        1'b0, CSR_MASK_ZERO};
      endcase
    end
  endfunction

  wire        req_valid   = req[CSR_REQ_VALID_BIT];
  wire [3:0]  req_func    = req[CSR_REQ_FUNC_MSB:CSR_REQ_FUNC_LSB];
  wire [11:0] req_addr    = req[CSR_REQ_ADDR_MSB:CSR_REQ_ADDR_LSB];
  wire        req_rd_is_a = req[CSR_REQ_RD_IS_A_BIT];
  wire [2:0]  req_rd_idx  = req[CSR_REQ_RD_IDX_MSB:CSR_REQ_RD_IDX_LSB];
  wire [47:0] req_src     = req[CSR_REQ_SRC_MSB:CSR_REQ_SRC_LSB];
  wire [1:0]  req_priv    = req[CSR_REQ_PRIV_MSB:CSR_REQ_PRIV_LSB];

  reg [CSR_RESP_WIDTH-1:0] resp_r;
  reg [CSR_WB_WIDTH-1:0]   writeback_r;

  assign resp = resp_r;
  assign writeback = writeback_r;

  reg [47:0] read_value;
  reg [47:0] write_value;
  reg        write_en;
  reg        illegal;
  reg        ok;

  reg [CSR_META_WIDTH-1:0]          meta;
  reg                               known;
  reg [CSR_META_PRIV_WIDTH-1:0]     required_priv;
  reg [CSR_META_READSEL_WIDTH-1:0]  read_sel;
  reg                               has_side_effect;
  reg [47:0]                        write_mask;
  reg [47:0]                        write_operand;
  reg                               write_allowed;

  always @* begin
    resp_r      = {CSR_RESP_WIDTH{1'b0}};
    writeback_r = {CSR_WB_WIDTH{1'b0}};
    read_value  = 48'd0;
    write_value = 48'd0;
    write_en    = 1'b0;
    illegal     = 1'b0;
    ok          = 1'b0;

    if (req_valid) begin
      meta           = csr_lookup_meta(req_addr);
      known          = meta[CSR_META_VALID_BIT];
      required_priv  = meta[CSR_META_PRIV_MSB:CSR_META_PRIV_LSB];
      read_sel       = meta[CSR_META_READSEL_MSB:CSR_META_READSEL_LSB];
      has_side_effect= meta[CSR_META_WRITE_SIDE_BIT];
      write_mask     = meta[CSR_META_WRITE_MASK_MSB:CSR_META_WRITE_MASK_LSB];
      write_operand  = req_src & write_mask;
      write_allowed  = has_side_effect || (write_mask != CSR_MASK_ZERO);

      case (read_sel)
        CSR_SEL_STATUS: begin
          read_value = csr_status;
          read_value[1:0] = req_priv;
        end
        CSR_SEL_SCRATCH: begin
          read_value = csr_scratch;
        end
        CSR_SEL_EPC: begin
          read_value = csr_epc;
        end
        CSR_SEL_CAUSE: begin
          read_value = csr_cause;
        end
        CSR_SEL_LR: begin
          read_value = csr_lr;
        end
        CSR_SEL_SSP: begin
          read_value = csr_ssp;
        end
        CSR_SEL_IRQ_ENABLE: begin
          read_value = csr_irq_enable;
        end
        CSR_SEL_IRQ_PENDING: begin
          read_value = csr_irq_pending;
        end
        CSR_SEL_IRQ_VECTOR: begin
          read_value = csr_irq_vector;
        end
        CSR_SEL_CYCLE: begin
          read_value = csr_cycle;
        end
        CSR_SEL_INSTRET: begin
          read_value = csr_instret;
        end
        CSR_SEL_TIMER: begin
          read_value = csr_timer;
        end
        CSR_SEL_TIMER_CMP: begin
          read_value = csr_timer_cmp;
        end
        default: begin
          read_value = 48'd0;
        end
      endcase

      ok      = known && (req_priv >= required_priv);
      illegal = known && !ok;

      case (req_func)
        CSR_F_RW: begin
          if (ok) begin
            if (write_allowed) begin
              write_en    = 1'b1;
              write_value = (read_value & ~write_mask) | write_operand;
              illegal     = 1'b0;
            end else begin
              illegal     = known;
            end
          end
        end
        CSR_F_RS: begin
          if (ok && (write_operand != 48'd0)) begin
            if (write_allowed) begin
              write_en    = 1'b1;
              write_value = read_value | write_operand;
              illegal     = 1'b0;
            end else begin
              illegal     = known;
            end
          end
        end
        CSR_F_RC: begin
          if (ok && (write_operand != 48'd0)) begin
            if (write_allowed) begin
              write_en    = 1'b1;
              write_value = read_value & ~write_operand;
              illegal     = 1'b0;
            end else begin
              illegal     = known;
            end
          end
        end
        CSR_F_R: begin
          if (ok) begin
            illegal = 1'b0;
          end
        end
        default: begin
          illegal  = 1'b1;
          write_en = 1'b0;
        end
      endcase

      if (ok && !illegal) begin
        if (req_rd_is_a) begin
          if (req_rd_idx != 3'd0) begin
            writeback_r[CSR_WB_VALID_BIT] = 1'b1;
            writeback_r[CSR_WB_IS_A_BIT]  = 1'b1;
            writeback_r[CSR_WB_IDX_MSB:CSR_WB_IDX_LSB] = req_rd_idx;
            writeback_r[CSR_WB_DATA_MSB:CSR_WB_DATA_LSB] = read_value;
          end
        end else begin
          writeback_r[CSR_WB_VALID_BIT] = 1'b1;
          writeback_r[CSR_WB_IS_A_BIT]  = 1'b0;
          writeback_r[CSR_WB_IDX_MSB:CSR_WB_IDX_LSB] = req_rd_idx;
          writeback_r[CSR_WB_DATA_MSB:CSR_WB_DATA_LSB] = read_value;
        end
      end

      resp_r[CSR_RESP_OK_BIT]       = ok;
      resp_r[CSR_RESP_ILLEGAL_BIT]  = illegal;
      resp_r[CSR_RESP_WRITE_EN_BIT] = write_en;
      resp_r[CSR_RESP_READ_MSB:CSR_RESP_READ_LSB]   = read_value;
      resp_r[CSR_RESP_WRITE_MSB:CSR_RESP_WRITE_LSB] = write_value;
    end
  end

endmodule
