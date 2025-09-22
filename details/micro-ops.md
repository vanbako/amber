# Amber48 Micro-ops

Amber48 keeps programmer-visible semantics in the lowercase ISA mnemonics while
the core executes sequences of uppercase micro-ops that resemble classic
assembly names. This document captures those internal micro-ops so the ISA spec
(`details/isa.md`) can stay focused on architectural behaviour.

## Naming & Scope

Micro-ops always appear in **UPPERCASE** to make the split obvious. The table
summarises the baseline catalogue and the ISA instructions that typically emit
them.

| Micro-op | Description | ISA sources |
|----------|-------------|-------------|
| `NOP` | Bubble inserted for hazard recovery or trap entry padding. | `no_oper`, pipeline controls |
| `MOV` | Register-to-register copy or capability pointer move. | `copy`, `cap_copy` |
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
| `CMP` | Compares two operands and sets NZCV. | `compare`, `compare_imm` |
| `TST` | Tests operand vs zero, updates flags without writeback. | `is_zero`, conditional branches |
| `LAND` | Hardware flow-control prove/check using `land tok9`. | `land`, indirect control flow |
| `BRA` | Unconditional branch with syllable target. | `branch` |
| `BCC` | Conditional branch using NZCV state. | `branch_cond`, packed compare results |
| `JMP` | Capability-checked jump to absolute address. | `jump`, `branch_sub` tail |
| `JCC` | Conditional jump/call with capability check. | `jump_cond`, `cap_branch_cond` |
| `PUSH` | Capability-safe stack/store of BAU payloads. | `push`, `jump_sub` prologue |
| `POP` | Capability-safe stack/load of BAU payloads. | `pop`, `return` epilogue |
| `RET` | Capability jump that restores LR/SSP. | `return` |
| `TRAP` | Raises synchronous trap with captured state. | `sys_call`, faults |

Micro-ops can be combined; a single ISA instruction can map to multiple entries
(e.g., `jump_sub` emits `PUSH`, `LAND`, `JMP`).

## Pipeline Contract

* Micro-ops issue out of ID and retire in WB in a single cycle when caches hit.
* Cache misses or explicit fences hold MA/MF, stalling the dependent micro-ops.
* XT still enforces syllable alignment; control-flow micro-ops (`BRA`, `JMP`,
  `JCC`, `RET`) never bypass the rule that targets land on bundle slot 0.

## Call & Stack Sequences

Amber48 decomposes complex ISA helpers into short, ordered micro-op runs:

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
