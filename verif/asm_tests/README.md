# AD48 Assembly Testsets

Self-checking assembly programs that exercise the AD48 ISA through the Python toolchain and Verilog simulation.

- `alu.asm` -- arithmetic, logical, shift, and immediate edge cases (swap variants, A0 write suppression).
- `memory.asm` -- load/store offsets, post-increment behaviour, negative displacements, and A0 suppression.
- `control.asm` -- conditional branches (all conditions, forward/backward offsets), JAL/JALR link paths, and system opcodes.
- `csr.asm` -- CSR file coverage: read/write/set/clear flows, counter seeding, privilege gating, and invalid accesses.
- `timer.asm` -- machine timer counter/compare CSRs, timer-driven interrupts, and handler re-arming semantics.

Run a single test via:

```powershell
python tools/ad48_toolchain.py verif/asm_tests/alu.toml --run-sim
```

Or execute every manifest:

```powershell
python tools/run_asm_tests.py
```
