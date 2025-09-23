#!/usr/bin/env bash
set -euo pipefail

# Run the Amber48 testbenches under Verilator.
ROOT_DIR="$(cd "$(dirname "$0")/.." && pwd)"
cd "$ROOT_DIR"

VERILATOR_FLAGS=(
  --binary
  --timescale 1ns/1ps
  -sv
  -Isrc/0.1/sim/tb
)

SOURCES=(
  src/0.1/rtl/common/amber48_pkg.sv
  src/0.1/rtl/core/amber48_core.sv
  src/0.1/rtl/core/amber48_decoder.sv
  src/0.1/rtl/core/amber48_regfile.sv
  src/0.1/rtl/core/amber48_alu.sv
  src/0.1/rtl/mem/amber48_imem.sv
  src/0.1/rtl/mem/amber48_dmem.sv
  src/0.1/rtl/periph/amber48_uart_tx.sv
  src/0.1/rtl/platform/reset_sync.sv
  src/0.1/sim/tb/amber48_core_tb.sv
)

STUBS=(
  "tests/add_loop.s:build/amber48_smoke.hex"
  "tests/branch_loop.s:build/amber48_branch.hex"
  "tests/misaligned_store.s:build/amber48_misaligned.hex"
  "tests/alu_ops.s:build/amber48_alu.hex"
  "tests/branch_edge.s:build/amber48_branch_ext.hex"
  "tests/call_return.s:build/amber48_call.hex"
  "tests/misaligned_load.s:build/amber48_misaligned_load.hex"
  "tests/dmem_oob.s:build/amber48_oob.hex"
)

if [ "$#" -gt 0 ]; then
  MODULES=("$@")
else
  MODULES=(
    amber48_core_smoke_tb
    amber48_core_branch_tb
    amber48_core_alu_tb
    amber48_core_misaligned_tb
    amber48_core_misaligned_load_tb
    amber48_core_oob_tb
  )
fi

for mapping in "${STUBS[@]}"; do
  IFS=":" read -r stub out <<<"$mapping"
  echo "[asm] $stub -> $out"
  python3 tools/asm_stub.py "$stub" -o "$out"
done

for top in "${MODULES[@]}"; do
  echo "[verilator] Building $top"
  verilator "${VERILATOR_FLAGS[@]}" --top-module "$top" "${SOURCES[@]}"
  echo "[verilator] Running $top"
  "./obj_dir/V${top}"
  echo
done
