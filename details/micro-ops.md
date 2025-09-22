# Amber48 Micro-ops

Amber48 keeps programmer-visible semantics in the lowercase ISA mnemonics while
the core executes sequences of uppercase micro-ops that resemble classic
assembly names. This document captures those internal micro-ops so the ISA spec
(`details/isa.md`) can stay focused on architectural behaviour.

## Naming & Scope

Micro-ops always appear in **UPPERCASE** to make the split obvious. The table
summarises the baseline catalogue and the ISA instructions that typically emit
them.

### Register Sets

Amber48 exposes four programmer-visible register families. Micro-ops stay within
one family unless noted, letting the register file and capability pipelines
optimise for operand width and metadata.

- **Data registers (`r0..r15`)** hold 48-bit integer or packed-lane values.
  `r0` is the architectural zero; `r1..r13` are general-purpose. `r14`/`r15`
  alias `SP`/`LR` when code needs a scalar view, but their architected updates
  still retire through the special-register path so the stack and return
  pointers keep their capability metadata.
- **Capability registers (`c0..c7`)** carry 96-bit CHERI-style pointers plus a
  tag. `c0` (`cPCC`) and `c1` (`cDDC`) are reserved; `c2..c7` are allocatable.
  Capability micro-ops preserve bounds, permissions, and tags; integer micro-ops
  never alias into this bank.
- **Control and Status Registers (CSRs)** capture privileged machine state such
  as `STATUS`, `EPC`, `TLBCFG`, and `IVEC`. Only the `csr_*` micro-ops read or
  write them, and the results bypass the integer and capability rename queues.
- **Special registers (SRs)** are always-live values that steer control flow:
  the fetch cursor (`PC` via `cPCC`), `SP`, `LR`, the hardware flow-control
  `SSP`, and the `STATE`/`NZCV` flags. The pointer-flavoured SRs (`PC`, `SP`,
  `LR`, `SSP`) are materialised as capabilities so traps, calls, and stack
  updates inherit bounds and permissions automatically; the flag SR remains a
  narrow, non-capability value.

### Baseline Catalogue

| Micro-op | Description | ISA sources |
|----------|-------------|-------------|
| `NOP` | Bubble inserted for hazard recovery or trap entry padding. | `no_oper`, pipeline controls |
| `MOV` | Copies within the data (`r`) bank or between capability (`c`) registers while preserving the tag. | `copy`, `cap_copy` |
| `NEG` | Two's-complement negate. | `negate`, `pack_negate.s/.u` |
| `SUB` | Arithmetic subtract (scalar or packed lane). | `subtract`, `subtract_imm`, packed variants |
| `ADD` | Arithmetic add (scalar or packed lane). | `add`, `add_imm`, packed variants |
| `NOT` | Bitwise invert. | `not`, `pack_not` |
| `AND` | Bitwise and / capability guard mask. | `and`, `cap_guard`, packed operations |
| `OR` | Bitwise or. | `or`, packed variants |
| `XOR` | Bitwise xor. | `xor`, packed variants |
| `LSL` | Logical left shift. | `ls_left`, `pack_ls_left` |
| `LSR` | Logical right shift. | `ls_right`, `pack_ls_right` |
| `ASR` | Arithmetic right shift. | `as_right`, `pack_as_right` |
| `ROL` | Rotate left. | `rot_left`, `pack_rot_left` |
| `ROR` | Rotate right. | `rot_right`, `pack_rot_right` |
| `CMP` | Compares two data operands and updates the `STATE/NZCV` special register. | `compare`, `compare_imm` |
| `TST` | Tests operand vs zero and updates the `STATE/NZCV` special register without writeback. | `is_zero`, conditional branches |
| `LAND` | Hardware flow-control prove/check using `land tok9`, consuming the `SSP` capability SR. | `land`, indirect control flow |
| `BRA` | Unconditional branch with syllable target, updating the `PC` capability via `cPCC`. | `branch` |
| `BCC` | Conditional branch reading `STATE/NZCV` to steer the `PC` capability. | `branch_cond`, packed compare results |
| `JMP` | Capability-checked jump that installs a new `cPCC`/`PC` from the capability bank or an SR alias. | `jump`, `branch_sub` tail |
| `JCC` | Conditional jump/call with capability check, sourcing bounds from the capability bank and retiring through the SR path. | `jump_cond`, `cap_branch_cond` |
| `PUSH` | Capability-safe stack/store of BAU payloads while updating the `SP`/`SSP` capability SRs. | `push`, `jump_sub` prologue |
| `POP` | Capability-safe stack/load of BAU payloads while updating the `SP`/`SSP` capability SRs. | `pop`, `return` epilogue |
| `RET` | Capability jump that restores `LR` and `SSP`, re-establishing `cPCC`. | `return` |
| `TRAP` | Raises synchronous trap, capturing `PC`/`cPCC` and staging results into CSRs (`EPC`, `CAUSE`, `STATUS`). | `sys_call`, faults |

Micro-ops can be combined; a single ISA instruction can map to multiple entries
(e.g., `jump_sub` emits `PUSH`, `LAND`, `JMP`).

## Pipeline Contract

* Micro-ops issue out of ID and retire in WB in a single cycle when caches hit.
* Cache misses or explicit fences hold MA/MF, stalling the dependent micro-ops.
* XT still enforces syllable alignment; control-flow micro-ops (`BRA`, `JMP`,
  `JCC`, `RET`) never bypass the rule that targets land on bundle slot 0.

## Call & Stack Sequences

Call-oriented ISA helpers straddle multiple register families: they read
capabilities from the `c` bank, update the `SP`/`LR`/`SSP` special registers,
and may stage trap metadata into CSRs. Amber48 decomposes them into short,
ordered micro-op runs:

* **`jump_sub`** → `PUSH` (save return capability), `LAND` (HFCP token check),
  `JMP` (capability branch to callee).
* **`return`** → `LAND` (revalidate target), `POP` (restore stack frame), `RET`
  (capability jump back to caller).
* **`push`/`pop`** → `PUSH` / `POP` ensure stack capability bounds update in lock
  step with the data movement so no torn frame becomes architecturally visible.

Because these sequences run under the hood, the ISA avoids multi-cycle encodings
for call/return or stack maintenance while still providing strict capability
checks.

## Trap & System Sequencing

* **`sys_call`** emits `TRAP`, capturing pipeline state and raising the synchronous
  `Syscall` cause without touching the interrupt vector path. Any `LAND`
  requirement for indirect entry is handled before the trap micro-op fires.
* Capability faults, MMU misses, or other synchronous exceptions also culminate
  in `TRAP`; the exact preamble micro-ops (e.g., `MOV` for CSR staging) depend on
  the faulting unit.

---

Future features (queued multipliers, capability compression, etc.) should extend
this document with additional uppercase micro-ops so the architectural and
micro-architectural narratives stay clearly separated.
