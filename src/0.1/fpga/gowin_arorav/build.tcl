# Amber48 0.1 Gowin Arora-V build script
set proj_dir [file normalize [pwd]]
set proj_name amber48_top
set impl_name impl_1
set top_module amber48_top

# Device settings (Gowin Arora-V 138k, PG484A package, version B, speed C1, temp I0)
set device_pn_base      GW5AST-LV138PG484A
set device_version      B
set device_speed_temp   C1/I0

# Compose identifiers for project creation and device selection
set device_pn_create    ${device_pn_base}${device_speed_temp}
set device_pn_select    ${device_pn_base}${device_speed_temp}

if {[file exists ${proj_dir}/${proj_name}.gpr]} {
  file delete -force ${proj_dir}/${proj_name}.gpr
}

create_project -name ${proj_name} -dir ${proj_dir} -pn ${device_pn_create} -device_version ${device_version} -force
set_device -device_version ${device_version} ${device_pn_select}
set_option -top_module ${top_module}
set_option -verilog_std sysv2017

# Generate ROM hex for instruction memory so UART prints PASS on boot.
# We invoke the repository's Python assembler to produce build/amber48_smoke.hex
# in the current project directory ([pwd] changes to the project after creation).
set asm_py   [file join ${proj_dir} tools asm_stub.py]
set asm_stub [file join ${proj_dir} tests 0.1 asm add_loop.s]
set rom_hex  [file join [pwd] build amber48_smoke.hex]
if {[file exists $asm_py]} {
  file mkdir [file dirname $rom_hex]
  set python_cmds [list [list py -3] python python3]
  set asm_ok 0
  foreach pcmd $python_cmds {
    if {$asm_ok} { break }
    if {![catch {exec {*}$pcmd $asm_py $asm_stub -o $rom_hex} msg]} {
      puts "Assembled ROM to $rom_hex"
      set asm_ok 1
    }
  }
  if {!$asm_ok} {
    # Fallback: if a prebuilt hex exists at repo root, copy it into project build dir.
    set root_hex [file join ${proj_dir} build amber48_smoke.hex]
    if {[file exists $root_hex]} {
      file mkdir [file dirname $rom_hex]
      file copy -force $root_hex $rom_hex
      puts "Copied ROM from $root_hex to $rom_hex"
    } else {
      puts "WARNING: Python not found and no prebuilt ROM at $root_hex; continuing without init file."
    }
  }
} else {
  puts "WARNING: Assembler not found at $asm_py; continuing without init file."
}

set rtl_files {
  src/0.1/rtl/common/amber48_pkg.sv
  src/0.1/rtl/core/amber48_core.sv
  src/0.1/rtl/core/amber48_alu.sv
  src/0.1/rtl/core/amber48_decoder.sv
  src/0.1/rtl/core/amber48_regfile.sv
  src/0.1/rtl/mem/amber48_imem.sv
  src/0.1/rtl/mem/amber48_dmem.sv
  src/0.1/rtl/periph/amber48_uart_tx.sv
  src/0.1/rtl/platform/reset_sync.sv
  src/0.1/fpga/gowin_arorav/amber48_top.sv
}
foreach f ${rtl_files} {
  set fp [file join ${proj_dir} $f]
  add_file -type verilog $fp
}

set pdc_file [file join ${proj_dir} "src/0.1/fpga/gowin_arorav/amber48_top.pdc"]
add_file ${pdc_file}

# Timing constraints (SDC): define the external 27 MHz sys_clk
set sdc_file [file join ${proj_dir} "src/0.1/fpga/gowin_arorav/amber48_top.sdc"]
if {[file exists ${sdc_file}]} {
  add_file -type sdc ${sdc_file}
}

run all
