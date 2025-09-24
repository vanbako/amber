package amber128_pkg;

  // Data and capability widths
  localparam int unsigned D_XLEN              = 64;   // data regs
  localparam int unsigned C_XLEN              = 128;  // capabilities & 128-bit memory word
  localparam int unsigned IMEM_WORD_BYTES     = 16;   // 128-bit instruction fetch window
  localparam int unsigned DMEM_WORD_BYTES     = 16;   // 128-bit load/store granularity

  // Register files
  localparam int unsigned DATA_REG_COUNT      = 16;   // r0..r15 (r0 is hardwired zero)
  localparam int unsigned DATA_REG_AW         = $clog2(DATA_REG_COUNT);
  localparam int unsigned CAP_REG_COUNT       = 8;    // c0..c7 (PC, SP, SSP, LR, etc.)
  localparam int unsigned CAP_REG_AW          = $clog2(CAP_REG_COUNT);
  localparam int unsigned CSR_COUNT           = 128;  // 128x CSR, 64-bit each
  localparam int unsigned CSR_AW              = $clog2(CSR_COUNT);

  // Special capability register indices (convention)
  localparam logic [CAP_REG_AW-1:0] CREG_PC   = CAP_REG_AW'(0);
  localparam logic [CAP_REG_AW-1:0] CREG_SP   = CAP_REG_AW'(1);
  localparam logic [CAP_REG_AW-1:0] CREG_SSP  = CAP_REG_AW'(2);
  localparam logic [CAP_REG_AW-1:0] CREG_LR   = CAP_REG_AW'(3);
  localparam logic [CAP_REG_AW-1:0] CREG_DDC  = CAP_REG_AW'(4); // default data cap
  localparam logic [CAP_REG_AW-1:0] CREG_PSTATE = CAP_REG_AW'(7);

  // Data register aliases
  localparam logic [DATA_REG_AW-1:0] REG_ZERO = '0;

  // Simple ALU ops (64-bit)
  typedef enum logic [3:0] {
    ALU_PASS,
    ALU_ADD,
    ALU_SUB,
    ALU_AND,
    ALU_OR,
    ALU_XOR,
    ALU_LSL,
    ALU_LSR
  } amber128_alu_op_e;

  typedef enum logic [2:0] {
    BR_NONE,
    BR_UNCOND,
    BR_EQ,
    BR_NE
  } amber128_branch_e;

  typedef enum logic [2:0] {
    TRAP_NONE,
    TRAP_ILLEGAL,
    TRAP_DATA_FAULT
  } amber128_trap_e;

  // Fetch bundle from IMEM: 128-bit window aligned to 16-byte boundary
  typedef struct packed {
    logic                   valid;
    logic [63:0]           word_addr;   // byte address aligned to 16 bytes
    logic [C_XLEN-1:0]     bundle;      // [127:123]=slot flags (5b), [119:0]=5 slots x 24b payload
  } amber128_fetch_s;

  // Decoded instruction (normalized to 64-bit datapath)
  typedef struct packed {
    logic                         valid;
    logic [63:0]                 pc_word_addr;   // bundle address
    logic [2:0]                  slot_idx;       // 0..4
    logic                        sub12_idx;      // 0 or 1 when slot encodes two 12-bit ops
    logic [DATA_REG_AW-1:0]      rs;             // source (12-bit forms map to r0..r7)
    logic [DATA_REG_AW-1:0]      rd;             // destination
    amber128_alu_op_e            alu_op;
    logic                        is_imm;
    logic [23:0]                 imm24;          // for 24-bit forms
    amber128_branch_e            branch;
    logic                        is_ld128;
    logic                        is_st128;
    logic [CAP_REG_AW-1:0]       cap_addr_sel;   // addressing capability (base/bounds)
    logic [CAP_REG_AW-1:0]       cap_data_sel;   // data capability for ld/st payload
  } amber128_decode_s;

  // Execute input
  typedef struct packed {
    logic                         valid;
    logic [63:0]                 pc_word_addr;
    logic [D_XLEN-1:0]           op_a;
    logic [D_XLEN-1:0]           op_b;
    logic [DATA_REG_AW-1:0]      rd;
    amber128_alu_op_e            alu_op;
    logic                        is_imm;
    logic [23:0]                 imm24;
    amber128_branch_e            branch;
    logic                        is_ld128;
    logic                        is_st128;
    logic [CAP_REG_AW-1:0]       _unused_cap;
  } amber128_exec_in_s;

  typedef struct packed {
    logic                         valid;
    logic [D_XLEN-1:0]           result;
    logic [DATA_REG_AW-1:0]      rd;
    logic                        wb_en;
    // Mem interface
    logic                        mem_req;
    logic                        mem_we;
    logic [63:0]                 mem_addr;  // byte addr (aligned to 16)
    logic [C_XLEN-1:0]           mem_wdata;
    // Branching
    logic                        branch_taken;
    logic [63:0]                 next_word_addr;
    // Traps
    logic                        trap;
    amber128_trap_e              trap_cause;
  } amber128_exec_out_s;

  // Regfile access
  typedef struct packed {
    logic                         valid;
    logic [DATA_REG_AW-1:0]      ra;
    logic [DATA_REG_AW-1:0]      rb;
    logic [DATA_REG_AW-1:0]      rw;
    logic                        we;
    logic [D_XLEN-1:0]           wd;
  } amber128_regfile_req_s;

endpackage : amber128_pkg
