# Amber48 Design Overview

Amber48 is a clean‑slate 48‑bit architecture built around Base Address Units (BAUs) instead of legacy byte cells. The platform treats BAUs as both the fundamental addressing quantum and the unit of execution for scalar and packed operations. This repository keeps the top‑level design concise and points to focused documents for details.

## Core Principles

- BAU‑native data model: Every architectural address, cache line, and memory operation works on 48‑bit BAUs with optional packed 2×24‑bit lanes.
- Capability‑first security: CHERI‑style capabilities gate all code and data access, including device DMA and inter‑module communication.
- Composable fabrics: The enid SerDes fabric provides PCIe‑like ordering guarantees while remaining BAU‑aligned and capability authenticated.
- Toolchain pragmatism: Assemblers and compilers target BAU semantics directly; lane selection, packed arithmetic, and wide immediates are explicit architectural concepts.

## Document Map

- `design/isa.md` — Instruction set, assembler conventions, memory model, and packed arithmetic behavior.
- `design/system.md` — Control/status registers, MMU layout, interrupt fabric, caches, pipeline timing, and hazard guidance.
- `design/capabilities.md` — Capability storage rules and lane‑aware pointer math for 24‑bit data structures.
- `design/abi.md` — Calling convention, stack discipline, and representative assembly snippets.
- `design/checklist.md` — Bring‑up checklist for a minimal Amber48 hardware/software stack.
- `design/glossary.md` — Acronyms and terminology used throughout the documents.

## High‑Level Roadmap

1. Finalize the MVP core configuration (scalar + packed ALUs, capability pipeline, baseline caches).
2. Stand up the enid root complex, MSI bridge, and driver model needed for early modules.
3. Deliver compiler/assembler support that respects BAU alignment, packed lanes, and capability calling conventions.
4. Iterate on coherency and acceleration features once the BAU‑native baseline is proven in silicon and software.

Each detail document remains self‑contained for deep dives; this overview stays short and adjusts only as the architectural pillars shift.

## Amber128 0.2 (At a Glance)

Amber 0.2 introduces a 128‑bit word architecture with 12/24‑bit instructions packed into 128‑bit bundles and an 8×128‑bit capability model. See:

- `design/0.2_isa.md` — 5‑slot bundle format, 12/24 encodings, 16‑byte‑aligned memory forms.
- `design/0.2_capabilities.md` — 128‑bit capability representation, base/bound checks, CARs (PC, SP, SSP, LR, DDC).
- `implementation/0.2_amber128.md` — RTL overview for `src/0.2` (core, decoder, capfile, IMEM/DMEM, FPGA top).

