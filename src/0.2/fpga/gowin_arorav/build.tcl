# Amber128 0.2 Gowin Arora-V build script
set proj_dir [file normalize [pwd]]
set proj_name amber128_top
set impl_name impl_1
set top_module amber128_top

# Device settings (Gowin Arora-V 138k, PG484A package, version B, speed C1, temp I0)
set device_pn_base      GW5AST-LV138PG484A
set device_version      B
set device_speed_temp   C1/I0

set device_pn_create    ${device_pn_base}${device_speed_temp}
set device_pn_select    ${device_pn_base}${device_speed_temp}

if {[file exists ${proj_dir}/${proj_name}.gpr]} {
  file delete -force ${proj_dir}/${proj_name}.gpr
}

create_project -name ${proj_name} -dir ${proj_dir} -pn ${device_pn_create} -device_version ${device_version} -force
set_device -device_version ${device_version} ${device_pn_select}
set_option -top_module ${top_module}
set_option -verilog_std sysv2017

# Generate ROM hex for instruction memory placeholder
set rom_hex  [file join [pwd] build amber128_smoke.hex]
if {![file exists $rom_hex]} {
  file mkdir [file dirname $rom_hex]
  # Minimal placeholder: one 128b word containing an unconditional 24b branch in slot 0
  # Layout: [127:123] flags=0, [122:120] reserved=0, [119:96] slot0=0x80_0000 (opcode=8, imm=0), others zero
  set fh [open $rom_hex w]
  puts $fh "00800000000000000000000000000000"
  close $fh
}

set rtl_files {
  src/0.1/rtl/platform/reset_sync.sv
  src/0.1/rtl/periph/amber48_uart_tx.sv
  src/0.2/rtl/common/amber128_pkg.sv
  src/0.2/rtl/core/amber128_regfile.sv
  src/0.2/rtl/core/amber128_capfile.sv
  src/0.2/rtl/core/amber128_decoder.sv
  src/0.2/rtl/core/amber128_alu.sv
  src/0.2/rtl/core/amber128_core.sv
  src/0.2/rtl/mem/amber128_imem.sv
  src/0.2/rtl/mem/amber128_dmem.sv
  src/0.2/fpga/gowin_arorav/amber128_top.sv
}
foreach f ${rtl_files} {
  set fp [file join ${proj_dir} $f]
  add_file -type verilog $fp
}

# Reuse the same PDC and SDC as 0.1 for board pinout and clock
set pdc_file [file join ${proj_dir} "src/0.1/fpga/gowin_arorav/amber48_top.pdc"]
add_file ${pdc_file}

set sdc_file [file join ${proj_dir} "src/0.1/fpga/gowin_arorav/amber48_top.sdc"]
if {[file exists ${sdc_file}]} {
  add_file -type sdc ${sdc_file}
}

run all
