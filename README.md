# Amber / AD48 Processor Project

Amber is an experimental 48-bit RISC processor core and SoC playground aimed at exploring modern OS concepts on FPGA-scale hardware. The current development snapshot (version `v0.2`) builds on the single-cycle Harvard baseline with maskable interrupts, richer CSR support, and expanded verification.

## Project Goals
- Deliver a clean, well-documented reference core suitable for teaching and rapid experimentation.
- Grow the instruction set, privilege model, and peripherals until a multi-tasking OS can run natively.
- Maintain a modular codebase that can scale from simple FPGA deployments to more capable SoC integrations.

## Current Capabilities (`v0.2` snapshot)
- 48-bit single-cycle CPU with ALU, load/store, branch, and system instruction classes.
- Dual register banks (`A` for addresses, `D` for data) with word-addressed IMEM/DMEM blocks.
- Machine CSR file (status with machine-interrupt enable, scratch, EPC/CAUSE, cycle/instret counters, IRQ enable/pending/vector registers) accessible through dedicated `csr.*` instructions.
- Maskable interrupt controller with latched pending bits, per-line enables, programmable vector base, and `SYS 0x2` (`iret`) for handler return.
- Self-checking testbenches covering ALU, control-flow, memory, exception, and interrupt behaviour.
- Tooling and documentation for assembling and loading AD48 programs.

## Roadmap
The milestone plan with ordered feature work is tracked in `doc/roadmap.md`. Each release layers OS-critical functionality such as trap handling, privilege separation, virtual memory, and I/O subsystems.

## Repository Layout
- `src/` - RTL for the CPU core, ALU, register files, on-chip memories, and SoC stubs.
- `doc/` - Architecture guides, assembler/toolchain notes, and the roadmap.
- `verif/` - Testbenches and verification infrastructure.
- `tools/` - Assembly, build, and simulation helpers.
- `build/`, `obj_dir/` - Generated artifacts from simulations or synthesis flows.

## Getting Started
1. Review `doc/cpu_ad48.md` for architectural details and instruction formats.
2. Use the tooling outlined in `doc/ad48_toolchain.md` to assemble and run sample programs.
3. Run the provided testbenches (see `verif/`) to validate changes before extending the core.

## Contributing / Next Steps
- Start with the `doc/roadmap.md` milestones to prioritize upcoming work.
- Keep RTL and documentation updates in sync; new features should arrive with verification coverage.
- Discussions, design notes, and experiment logs can live under `doc/` for future reference.
