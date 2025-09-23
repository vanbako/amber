`timescale 1ns/1ps

`define TB_MODULE_NAME amber48_core_smoke_tb
`define TB_NAME "smoke"
`define TB_IMEM_FILE "build/amber48_smoke.hex"
`define TB_DMEM_FILE ""
`define TB_EXPECTED_TRAP amber48_pkg::TRAP_ILLEGAL
`define TB_MAX_CYCLES 5000000
`define TB_EXPECT_STORE_ADDR0 1
`include "amber48_core_tb_base.svh"

`define TB_MODULE_NAME amber48_core_branch_tb
`define TB_NAME "branch"
`define TB_IMEM_FILE "build/amber48_branch.hex"
`define TB_DMEM_FILE ""
`define TB_EXPECTED_TRAP amber48_pkg::TRAP_ILLEGAL
`define TB_MAX_CYCLES 5000000
`define TB_EXPECT_STORE_ADDR0 0
`include "amber48_core_tb_base.svh"

`define TB_MODULE_NAME amber48_core_alu_tb
`define TB_NAME "alu"
`define TB_IMEM_FILE "build/amber48_alu.hex"
`define TB_DMEM_FILE ""
`define TB_EXPECTED_TRAP amber48_pkg::TRAP_ILLEGAL
`define TB_MAX_CYCLES 5000000
`define TB_EXPECT_STORE_ADDR0 1
`include "amber48_core_tb_base.svh"

`define TB_MODULE_NAME amber48_core_branch_ext_tb
`define TB_NAME "branch_ext"
`define TB_IMEM_FILE "build/amber48_branch_ext.hex"
`define TB_DMEM_FILE ""
`define TB_EXPECTED_TRAP amber48_pkg::TRAP_ILLEGAL
`define TB_MAX_CYCLES 5000000
`define TB_EXPECT_STORE_ADDR0 1
`include "amber48_core_tb_base.svh"

`define TB_MODULE_NAME amber48_core_call_tb
`define TB_NAME "call"
`define TB_IMEM_FILE "build/amber48_call.hex"
`define TB_DMEM_FILE ""
`define TB_EXPECTED_TRAP amber48_pkg::TRAP_ILLEGAL
`define TB_MAX_CYCLES 5000000
`define TB_EXPECT_STORE_ADDR0 1
`include "amber48_core_tb_base.svh"

`define TB_MODULE_NAME amber48_core_misaligned_tb
`define TB_NAME "misaligned_store"
`define TB_IMEM_FILE "build/amber48_misaligned.hex"
`define TB_DMEM_FILE ""
`define TB_EXPECTED_TRAP amber48_pkg::TRAP_DATA_FAULT
`define TB_MAX_CYCLES 5000000
`define TB_EXPECT_STORE_ADDR0 0
`include "amber48_core_tb_base.svh"

`define TB_MODULE_NAME amber48_core_misaligned_load_tb
`define TB_NAME "misaligned_load"
`define TB_IMEM_FILE "build/amber48_misaligned_load.hex"
`define TB_DMEM_FILE ""
`define TB_EXPECTED_TRAP amber48_pkg::TRAP_DATA_FAULT
`define TB_MAX_CYCLES 5000000
`define TB_EXPECT_STORE_ADDR0 0
`include "amber48_core_tb_base.svh"

`define TB_MODULE_NAME amber48_core_oob_tb
`define TB_NAME "dmem_oob"
`define TB_IMEM_FILE "build/amber48_oob.hex"
`define TB_DMEM_FILE ""
`define TB_EXPECTED_TRAP amber48_pkg::TRAP_DATA_FAULT
`define TB_MAX_CYCLES 5000000
`define TB_EXPECT_STORE_ADDR0 0
`include "amber48_core_tb_base.svh"
