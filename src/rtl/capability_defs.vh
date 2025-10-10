`ifndef CAPABILITY_DEFS_VH
`define CAPABILITY_DEFS_VH

// Common capability metadata definitions shared across the core.
`define CAP_ADDR_WIDTH 48
`define CAP_PERM_WIDTH 3

`define CAP_PERM_READ  3'b001
`define CAP_PERM_WRITE 3'b010
`define CAP_PERM_EXEC  3'b100
`define CAP_PERM_ALL   3'b111

`endif // CAPABILITY_DEFS_VH
