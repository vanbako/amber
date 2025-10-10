`timescale 1ns/1ps

// Compute data memory addressing and optional post-increment bookkeeping.
// This keeps the main execute block focused on instruction-level control
// while exposing the sum, bounds checking, and base register writeback hints
// as simple combinational outputs.
module mem_access_unit #(
  parameter integer WORD_WIDTH = 48,
  parameter integer DM_WORDS   = 16384,
  parameter integer ADDR_WIDTH = $clog2(DM_WORDS)
) (
  input  wire [WORD_WIDTH-1:0] base_value,
  input  wire [WORD_WIDTH-1:0] disp_value,
  input  wire [2:0]            base_index,
  input  wire                  post_increment,
  output wire [ADDR_WIDTH-1:0] address_index,
  output wire                  address_invalid,
  output wire                  post_update_en,
  output wire [2:0]            post_update_idx,
  output wire [WORD_WIDTH-1:0] post_update_value
);
  localparam integer EXT_WIDTH = WORD_WIDTH + 1;

  wire [EXT_WIDTH-1:0] base_ext = {1'b0, base_value};
  wire [EXT_WIDTH-1:0] disp_ext = {disp_value[WORD_WIDTH-1], disp_value};
  wire [EXT_WIDTH-1:0] addr_ext = base_ext + disp_ext;

  assign post_update_value = addr_ext[WORD_WIDTH-1:0];
  assign address_index     = addr_ext[ADDR_WIDTH-1:0];
  assign address_invalid   = addr_ext[WORD_WIDTH] || (post_update_value >= DM_WORDS);
  assign post_update_en    = post_increment && (base_index != 3'd0);
  assign post_update_idx   = base_index;
endmodule
