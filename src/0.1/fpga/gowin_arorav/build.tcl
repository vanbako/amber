# Amber48 0.1 Gowin Arora-V build script
set proj_dir [file normalize [pwd]]
set proj_name amber48_top
set impl_name impl_1
set device_part GW5AST-LV138PG484AC1/I0
set top_module amber48_top

if {[file exists ${proj_dir}/${proj_name}.gpr]} {
  file delete -force ${proj_dir}/${proj_name}.gpr
}

create_project -name ${proj_name} -dir ${proj_dir} -device ${device_part}
set_option -use_relative_file_path 1
set_option -top_module ${top_module}
set_option -verilog_std sysv2012

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
  add_file -type systemverilog $f
}

add_file -type constraint "src/0.1/fpga/gowin_arorav/amber48_top.pdc"

save_project

run synthesize
run place
run route
run bitstream
