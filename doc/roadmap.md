# AD48 Platform Roadmap

Current release: `v0.1` (single-cycle baseline core).
In development: `v0.2` (privileged control surface) work has begun.

Each milestone below builds toward a protected, multitasking operating system running on the AD48 processor and associated SoC support logic. Features are ordered to minimize backtracking and to layer high-value OS-visible capabilities first.

## v0.1 - Baseline Core *(current)*
- Single-cycle Harvard CPU with ALU, load/store, branch, and system instruction classes.
- Separate `A`/`D` register banks with word-addressed IMEM/DMEM blocks.
- Simulation testbenches for ALU, control, and memory subsystems.

## v0.2 - Privileged Control Surface
- Control and Status Register (CSR) file with privilege-guarded read/write paths.
- Precise synchronous exceptions (illegal instruction, breakpoint, misaligned access).
- Interrupt entry/return flow, including external IRQ lines and a minimal interrupt controller.
- Machine timer and cycle counter CSRs for scheduler ticks and profiling support.

## v0.3 - Protected Execution Environment
- Hardware user/kernel privilege levels and mode bits in the CSR status register.
- Supervisor stack pointer (`SSP`) and dedicated link register semantics for trap/return.
- Refactoring
  1. factor ALU-immediate decode into a helper
  2. prototype a CSR metadata table
  3. split the big combinational block into themed sub-blocks or modules once helpers are in place ([refactoring plan](refactoring.md#control-block-decomposition)).
- Capability metadata (bounds/permissions tags) attached to pointer-carrying registers.
- System call ABI routing through the trap handler with context save/restore helpers.

## v0.4 - Instruction Set Extensions
- Translator that maps ISA macro-ops (e.g., `PUSH`, `POP`, `BSR`, `JSR`, `RET`) into micro-op sequences.
- Additional micro-op instructions to support stack manipulation, context management, and barriers.
- Architectural memory barrier/fence instructions for safe device and shared-memory access.
- Baseline atomic primitives (load-reserved/store-conditional or fetch-add) for lock-free OS kernels.

## v0.5 - Virtual Memory & Caching
- Memory Management Unit (MMU) with paged virtual address space and multi-level page tables.
- TLB management instructions plus page-fault exception integration with the trap handler.
- Split instruction/data caches with coherency rules suitable for DMA and atomic operations.
- Cache maintenance instructions (invalidate/clean) exposed via CSR or system opcodes.

## v0.6 - Microarchitecture Scaling
- Five-stage (or deeper) pipeline with hazard detection, forwarding, and precise exception maintenance.
- Compressed 24-bit instruction stacking within 48-bit words, including fetch/decode updates.
- Optional branch prediction or simple static prediction hooks for future performance tuning.
- Updated verification to cover multi-cycle timing, stalls, and flush scenarios.

## v0.7 - Platform I/O & DMA
- Memory-mapped I/O (MMIO) interconnect with address decoding and privilege checks.
- Direct Memory Access (DMA) engine with interrupt-driven completion.
- Standard peripheral block implementations (UART console, multi-channel timer, GPIO).
- Programmable interrupt controller (PIC) compatible with the OS interrupt model.

## v0.8 - External Memory & Storage
- SDRAM/DDR3 controller with initialization, refresh, and burst support.
- Storage interfaces (SD/eMMC, USB mass storage, SATA/SSD) with MMIO drivers.
- Boot strap logic to load firmware images from external memory or storage devices.
- Cache/MMU integration with external memory for coherent instruction/data fetches.

## v0.9 - Firmware & Boot Experience
- BIOS/bootloader that initializes hardware, configures CSRs, and loads the OS kernel.
- Device discovery tables handed off to the OS (memory map, interrupt layout, capabilities).
- Early console and debugging services available before the OS scheduler starts.
- Manufacturing/bring-up scripts for ROM image generation and deployment.

## v1.0 - System Integration & UX
- Text screen coprocessor with DMA-backed frame buffer for terminal and UI output.
- On-chip debug features: hardware breakpoints, trace buffers, and performance counters.
- Capability-aware loader and runtime checks integrated with the OS memory manager.
- Comprehensive documentation, compliance tests, and CI flow for regression gating.
