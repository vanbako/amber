# AD48 Core Refactoring Plan

This document captures the primary restructuring opportunities inside `src/rtl/cpu_ad48.v`. Each block outlines the motivation, targeted code span, and suggested extraction form (module/function/task).

## Utility Function Consolidation (implemented)
- **Scope:** `src/rtl/cpu_ad48.v:320-349` and scattered helper logic.
- **Pain point:** Ad-hoc helpers (e.g., ALU-immediate decode) live inside the core, preventing reuse and cluttering the top-level file.
- **Action:** Relocate decode tables, sign-extension helpers, and privilege sanitisation into a shared package or include file, preparing for multi-file modularisation.

## Decode Layer Helpers (implemented)
- **Scope:** `src/rtl/cpu_ad48.v:384-600`
- **Pain point:** Opcode cases repeat register writeback setup and ALU operand selection logic.
- **Action:** Introduce helper functions/tasks that accept decoded fields and return structured control bundles (e.g., writeback intents, ALU ops), enabling table-driven decode expansion.

## Branch & Jump Decision Helpers (implemented)
- **Scope:** `src/rtl/cpu_ad48.v:558-598`
- **Pain point:** Branch condition evaluation and jump link writeback logic intertwine with other decode cases.
- **Action:** Provide small helpers (`branch_take`, `jal_link_write`, `jalr_target_calc`) that encapsulate comparator usage and link semantics, paving the way for pipeline hazard integration.

## Memory Address & Post-Increment Sequencer (implemented)
- **Scope:** `src/rtl/cpu_ad48.v:525-555`
- **Pain point:** Address calculation, DMEM access staging, and optional post-increment updates mix arithmetic with side-effect decisions.
- **Action:** Extract to a `mem_access_unit` that returns address, write enables, and optional post-update data, simplifying addition of cache/MMU layers.

## Register Bank Write Arbitration (implemented)
- **Scope:** `src/rtl/cpu_ad48.v:517-720`
- **Pain point:** Decentralised gating for A/D write enables risks conflicts when multiple sources request writes in the same cycle.
- **Action:** Funnel all write intents through a small arbiter task/module that resolves priority and applies handler overrides (e.g., `handler_active` redirect for `SSP`).

## CSR Access Unit (implemented)
- **Scope:** `src/rtl/cpu_ad48.v:601-724`
- **Pain point:** CSR metadata lookup, privilege checks, read muxing, write masking, and side-effect management occupy a large contiguous region.
- **Action:** Wrap into a dedicated `csr_unit` module/function exposing a narrow interface (`req`, `resp`, `writeback`), centralising CSR semantics for reuse and isolated verification.

## Interrupt Priority Encoder (implemented)
- **Scope:** `src/rtl/cpu_ad48.v:430`
- **Pain point:** In-line loop searches for the first enabled IRQ, burying policy inside the main control block.
- **Action:** Extract to an `irq_priority` function/module returning `{found, index}`, making alternative ordering strategies or mask handling pluggable.

## IRQ Pending Bookkeeping (implemented)
- **Scope:** `src/rtl/cpu_ad48.v:912-919`
- **Pain point:** Masking and accumulation of pending IRQ bits is duplicated in combinational and sequential paths.
- **Action:** Factor out a shared function that takes current pending, write intents, and live signals, returning the next pending vector to keep policy consistent.

## Trap & Exception Sequencer (implemented)
- **Scope:** `src/rtl/cpu_ad48.v:766-805`
- **Pain point:** Exception/interrupt classification and trap vector selection are embedded mid-block, forcing control flow fall-through.
- **Action:** Create a `trap_sequencer` helper that consumes status flags and returns `{trap_taken, vector, cause, squash_controls}` to keep the parent logic linear.

## Sequential State Updates (implemented)
- **Scope:** `src/rtl/cpu_ad48.v:811-925`
- **Pain point:** The single sequential `always` updates PC, privilege, every CSR, counters, and IRQ bookkeeping, complicating reasoning about ordering.
- **Action:** Split into themed always blocks or tasks (`update_pc_branching`, `update_privilege_status`, `update_counters`, `update_irq_state`) to isolate dependencies and support future pipelines.

## Control Block Decomposition (planned)
- **Scope:** `src/rtl/cpu_ad48.v:384`
- **Pain point:** Single combinational `always @*` interleaves decode, ALU steering, branch handling, memory sequencing, CSR coordination, and trap prep.
- **Action:** Carve themed control sub-blocks (e.g., `decode_exec`, `mem_access_ctrl`, `csr_dispatch`, `trap_precheck`) as tasks or child modules to shrink the top-level case tree and ease incremental additions.

## Parameterised Memory Interfaces (planned)
- **Scope:** `src/rtl/mem48.v`, `src/rtl/mem_access_unit.v`, instantiations in `src/rtl/cpu_ad48.v`
- **Pain point:** Instruction and data memories share a single fixed implementation, making it hard to swap in caches, wider memories, or external buses.
- **Action:** Wrap the memory blocks behind parameterised modules or an interface layer (AXI-lite style signals, ready/valid handshake), so future SOC work can replace memories without touching the core control logic.

## Register File Module Extraction (planned)
- **Scope:** `src/rtl/regfiles.v`, register write/read wiring in `src/rtl/cpu_ad48.v`
- **Pain point:** Operand muxing and writeback rules still leak into the core; adding capabilities (extra banks, debug ports) means editing the main control block.
- **Action:** Promote the A/D register banks to standalone modules with explicit read/write intents, privilege hooks, and optional capability bits. Expose a narrow API so the core issues commands instead of manipulating raw arrays.

## ALU Micro-Modules (planned)
- **Scope:** `src/rtl/alu.v`, arithmetic/logic case tree inside `src/rtl/cpu_ad48.v`
- **Pain point:** A single monolithic case statement makes it cumbersome to add new ALU operations or introduce multi-cycle units.
- **Action:** Factor arithmetic, logical, shift/rotate, and compare families into sub-modules (or functions) that share a consistent request/response record. This allows incremental upgrades (e.g., multiplier, barrel shifter) without destabilising existing ops.

## System Call & Exception Layer (planned)
- **Scope:** Trap handling paths in `src/rtl/cpu_ad48.v`, CSR side effects in `src/rtl/csr_unit.v`
- **Pain point:** Special instructions like HALT are hard-coded during decode, conflating them with generic exception flow and limiting extensibility.
- **Action:** Route supervisor calls through the trap sequencer/CSR machinery by defining a dedicated syscall request channel and associated CSRs (`mcause`, `mtval` style). This keeps decode lightweight and paves the way for richer supervisor services.

## Interrupt Controller Module (planned)
- **Scope:** IRQ pending/masking logic in `src/rtl/cpu_ad48.v`, helper functions in `src/rtl/cpu_ad48_utils.vh`
- **Pain point:** Priority resolution and masking still live inside the core, preventing reuse and complicating experiments with nested/priority interrupts.
- **Action:** Extract an interrupt controller sub-module that owns pending bits, prioritisation, and acknowledgements. Feed its outputs into the trap sequencer so alternative policies (round-robin, vectored) can be dropped in.

## Pipeline Stage Boundary Definition (future exploration)
- **Scope:** Entire control/data path in `src/rtl/cpu_ad48.v`
- **Pain point:** Lack of explicit fetch/decode/execute boundaries blocks incremental pipelining and hampers hazard analysis.
- **Action:** Introduce clear pipeline registers (even if initially bypassed) between fetch, decode, execute, memory, and writeback flows. Document the signals that cross each boundary to prepare for staged execution and performance features (branch prediction, dual-issue experiments).
