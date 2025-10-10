`ifndef CPU_AD48_UTILS_VH
`define CPU_AD48_UTILS_VH

// Helper utilities shared across cpu_ad48 implementations.

// ALU immediate decode: returns {valid, imm_zero, alu_op}.
function automatic [7:0] cpu_ad48_decode_alui_subop;
  input [3:0] subop;
  begin
    case (subop)
      F_ADD: cpu_ad48_decode_alui_subop = {1'b1, 1'b0, 6'h00};
      F_AND: cpu_ad48_decode_alui_subop = {1'b1, 1'b0, 6'h02};
      F_OR : cpu_ad48_decode_alui_subop = {1'b1, 1'b0, 6'h03};
      F_XOR: cpu_ad48_decode_alui_subop = {1'b1, 1'b0, 6'h04};
      F_SLL: cpu_ad48_decode_alui_subop = {1'b1, 1'b0, 6'h05};
      F_SRL: cpu_ad48_decode_alui_subop = {1'b1, 1'b0, 6'h06};
      F_SRA: cpu_ad48_decode_alui_subop = {1'b1, 1'b0, 6'h07};
      F_NOT: cpu_ad48_decode_alui_subop = {1'b1, 1'b1, 6'h08};
      default: cpu_ad48_decode_alui_subop = {1'b0, 1'b0, 6'h00};
    endcase
  end
endfunction

// Sign-extension helpers for the common immediate widths.
function automatic [47:0] cpu_ad48_sx_imm27;
  input [26:0] imm;
  begin
    cpu_ad48_sx_imm27 = {{21{imm[26]}}, imm};
  end
endfunction

function automatic [47:0] cpu_ad48_sx_disp33;
  input [32:0] disp;
  begin
    cpu_ad48_sx_disp33 = {{15{disp[32]}}, disp};
  end
endfunction

function automatic [47:0] cpu_ad48_sx_br31;
  input [30:0] off;
  begin
    cpu_ad48_sx_br31 = {{17{off[30]}}, off};
  end
endfunction

function automatic [47:0] cpu_ad48_sx_jal36;
  input [35:0] off;
  begin
    cpu_ad48_sx_jal36 = {{12{off[35]}}, off};
  end
endfunction

function automatic [47:0] cpu_ad48_sx_jr33;
  input [32:0] imm;
  begin
    cpu_ad48_sx_jr33 = {{15{imm[32]}}, imm};
  end
endfunction

// Privilege and status register helpers.
function automatic [1:0] cpu_ad48_sanitize_priv_mode;
  input [1:0] mode_in;
  begin
    case (mode_in)
      PRIV_MACHINE:    cpu_ad48_sanitize_priv_mode = PRIV_MACHINE;
      PRIV_SUPERVISOR: cpu_ad48_sanitize_priv_mode = PRIV_SUPERVISOR;
      default:         cpu_ad48_sanitize_priv_mode = PRIV_USER;
    endcase
  end
endfunction

function automatic [1:0] cpu_ad48_status_prev_mode;
  input [47:0] status_in;
  begin
    cpu_ad48_status_prev_mode =
      cpu_ad48_sanitize_priv_mode(status_in[STATUS_PREV_MODE_MSB:STATUS_PREV_MODE_LSB]);
  end
endfunction

function automatic [47:0] cpu_ad48_normalize_status_write;
  input [47:0] write_value;
  input [1:0]  new_mode;
  reg   [47:0] status_out;
  begin
    status_out = write_value;
    status_out[1:0] = new_mode;
    status_out[STATUS_PREV_MODE_MSB:STATUS_PREV_MODE_LSB] =
      cpu_ad48_sanitize_priv_mode(write_value[STATUS_PREV_MODE_MSB:STATUS_PREV_MODE_LSB]);
    cpu_ad48_normalize_status_write = status_out;
  end
endfunction

function automatic [47:0] cpu_ad48_status_trap_transition;
  input [47:0] status_in;
  input [1:0]  mode_in;
  reg   [47:0] status_out;
  reg   [1:0]  sanitized_mode;
  begin
    status_out     = status_in;
    sanitized_mode = cpu_ad48_sanitize_priv_mode(mode_in);
    status_out[STATUS_PREV_MODE_MSB:STATUS_PREV_MODE_LSB] = sanitized_mode;
    case (sanitized_mode)
      PRIV_MACHINE:    status_out[STATUS_MPIE_BIT] = status_in[STATUS_MIE_BIT];
      PRIV_SUPERVISOR: status_out[STATUS_KPIE_BIT] = status_in[STATUS_KIE_BIT];
      default:         status_out[STATUS_UPIE_BIT] = status_in[STATUS_UIE_BIT];
    endcase
    status_out[STATUS_MPIE_BIT] = status_in[STATUS_MIE_BIT];
    status_out[STATUS_MIE_BIT]  = 1'b0;
    status_out[1:0]             = PRIV_MACHINE;
    cpu_ad48_status_trap_transition = status_out;
  end
endfunction

function automatic [47:0] cpu_ad48_status_iret_transition;
  input [47:0] status_in;
  reg   [47:0] status_out;
  reg   [1:0]  resume_mode;
  begin
    status_out  = status_in;
    resume_mode = cpu_ad48_status_prev_mode(status_in);
    status_out[1:0]            = resume_mode;
    status_out[STATUS_MIE_BIT] = status_in[STATUS_MPIE_BIT];
    case (resume_mode)
      PRIV_MACHINE: begin
        status_out[STATUS_MIE_BIT]  = status_in[STATUS_MPIE_BIT];
        status_out[STATUS_MPIE_BIT] = 1'b1;
      end
      PRIV_SUPERVISOR: begin
        status_out[STATUS_KIE_BIT]  = status_in[STATUS_KPIE_BIT];
        status_out[STATUS_KPIE_BIT] = 1'b1;
      end
      default: begin
        status_out[STATUS_UIE_BIT]  = status_in[STATUS_UPIE_BIT];
        status_out[STATUS_UPIE_BIT] = 1'b1;
      end
    endcase
    status_out[STATUS_MPIE_BIT] = 1'b1;
    status_out[STATUS_PREV_MODE_MSB:STATUS_PREV_MODE_LSB] = PRIV_USER;
    cpu_ad48_status_iret_transition = status_out;
  end
endfunction

`endif // CPU_AD48_UTILS_VH
