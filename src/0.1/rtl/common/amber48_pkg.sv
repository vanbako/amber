package amber48_pkg;

  localparam int unsigned XLEN              = 48;
  localparam int unsigned REG_COUNT         = 16;
  localparam int unsigned REG_ADDR_WIDTH    = $clog2(REG_COUNT);
  localparam int unsigned IMEM_DEPTH        = 256;
  localparam int unsigned DMEM_DEPTH        = 256;
  localparam int unsigned BAU_BYTES         = 8;  // 64-bit byte addressable unit

  typedef enum logic [3:0] {
    ALU_ADD,
    ALU_SUB,
    ALU_AND,
    ALU_OR,
    ALU_XOR,
    ALU_LSL,
    ALU_LSR,
    ALU_PASS
  } amber48_alu_op_e;

  typedef enum logic [3:0] {
    BR_NONE,
    BR_UNCOND,
    BR_EQ,
    BR_NE,
    BR_LT_U,
    BR_LT_S,
    BR_GT_U,
    BR_GT_S,
    BR_ZERO,
    BR_NOT_ZERO
  } amber48_branch_e;

  typedef enum logic [2:0] {
    TRAP_NONE,
    TRAP_ILLEGAL,
    TRAP_DATA_FAULT
  } amber48_trap_e;

  typedef struct packed {
    logic                      valid;
    logic [XLEN-1:0]           pc;
    logic [XLEN-1:0]           instr;
  } amber48_decode_in_s;

  typedef struct packed {
    logic                      valid;
    logic [XLEN-1:0]           pc;
    logic [REG_ADDR_WIDTH-1:0] rs1;
    logic [REG_ADDR_WIDTH-1:0] rs2;
    logic [REG_ADDR_WIDTH-1:0] rd;
    amber48_alu_op_e           alu_op;
    logic [XLEN-1:0]           imm;
    logic                      uses_imm;
    amber48_branch_e           branch_type;
    logic                      load;
    logic                      store;
    logic                      trap;
    amber48_trap_e             trap_cause;
  } amber48_decode_out_s;

  typedef struct packed {
    logic                      valid;
    logic [XLEN-1:0]           pc;
    logic [XLEN-1:0]           op_a;
    logic [XLEN-1:0]           op_b;
    amber48_alu_op_e           alu_op;
    logic [XLEN-1:0]           imm;
    logic                      uses_imm;
    amber48_branch_e           branch_type;
    logic [REG_ADDR_WIDTH-1:0] rd;
    logic [XLEN-1:0]           store_data;
    logic                      writeback_en;
    logic                      load;
    logic                      store;
    logic                      trap;
    amber48_trap_e             trap_cause;
  } amber48_execute_in_s;

  typedef struct packed {
    logic                      valid;
    logic [XLEN-1:0]           result;
    logic [REG_ADDR_WIDTH-1:0] rd;
    logic [XLEN-1:0]           store_data;
    logic                      writeback_en;
    logic                      load;
    logic                      store;
    logic                      branch_taken;
    logic [XLEN-1:0]           branch_target;
    logic                      trap;
    amber48_trap_e             trap_cause;
  } amber48_execute_out_s;

  typedef struct packed {
    logic                      valid;
    logic [REG_ADDR_WIDTH-1:0] addr_a;
    logic [REG_ADDR_WIDTH-1:0] addr_b;
    logic [REG_ADDR_WIDTH-1:0] addr_w;
    logic                      we;
    logic [XLEN-1:0]           wd;
  } amber48_regfile_req_s;

endpackage : amber48_pkg
