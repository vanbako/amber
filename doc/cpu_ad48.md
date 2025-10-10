# AD48 Processor Design & Implementation Guide

## Overview
- 48-bit, single-cycle Harvard CPU optimized for small FPGA/SOC experiments.
- Two independent register banks: address bank `A[7:0]` with `A0` hard-wired to zero, and data bank `D[7:0]`.
- Word-addressed memories; the program counter (PC) counts 48-bit words.
- Instruction ROM (`IMEM`) and data RAM (`DMEM`) are simple synchronous memories instantiated inside the core.
- Instruction set grouped into nine classes: ALU, ALU-immediate (A/D variants), load, store, branch, JAL, JALR, and system.

The intent is to keep timing simple (one instruction per cycle, aside from a HALT hold) while leaving hooks for future features like variable-latency memories or extended opcodes.

## Microarchitecture
- **Fetch stage**: `pc` indexes instruction memory inside `src/rtl/cpu_ad48.v`. The register holds a 48-bit word address and only the low bits required for the configured memory depth feed the memory instance.
- **Decode stage**: The same module performs a combinational decode of opcode class, register indices, funct/subop fields, branch conditions, and immediates.
- **Register access**: `regfileA`/`regfileD` modules provide single-read/single-write ports clocked on the rising edge (`src/rtl/regfiles.v`). `A0` ignores writes and always reads as zero to simplify base+offset addressing.
- **Execute/memory**: A shared ALU handles arithmetic/logic, immediate execution, address formation for loads/stores, and branch condition evaluation. Memory access paths route through `simple_mem48` instances for DMEM (`src/rtl/mem48.v`).
- **Writeback**: Register bank write enables and destinations are computed per instruction. LDs always target the D bank; other instructions can steer results to either bank via `rdBank`.
- **Control flow**: Branches, JAL, and JALR compute next PC values combinationally. JAL/JALR use `PC+1` as the link value to keep word addressing consistent.
- **System**: `SYS 0` acts as a NOP, `SYS 1` raises a breakpoint exception, `SYS 0x2` returns from an interrupt (restoring `STATUS`/`EPC`), and `SYS 0xF` halts the core by holding the PC and asserting the internal `halt` flag. All other encodings trap as illegal instructions.
- **CSR file**: Status, scratch, EPC/CAUSE, and counter CSRs live alongside the datapath. CSR instructions apply privilege checks, update counters automatically, and keep the architectural `STATUS` view in sync with the internal `priv_mode`.

Timing remains single-cycle: all combinational work completes between clock edges, and the registered PC advances every cycle except during reset or HALT. There is no pipeline and thus no hazard management logic.

## Programming Model

### Registers
- **A-bank** (`A0`..`A7`): intended for addresses/base pointers. `A0` is hard-wired to zero; post-increment is disabled when `baseA == A0` to keep the invariant. While a trap is active, accesses to `A7` are transparently steered to the supervisor stack pointer (`SSP`) so handlers can rely on familiar load/store idioms without mutating the user-mode value latched in the file.
- **D-bank** (`D0`..`D7`): general-purpose data registers. Loads always write to this bank; stores read from it.
- **PC**: 48-bit word index. `PC+1` denotes the next sequential instruction.
- **Link register (`LR`)**: dedicated CSR (`CSR_LR`) that mirrors `CSR_EPC`. Trap entry copies the resume address into `LR`, and `IRET` returns to its value. Handlers read or update it via `csr.read lr` / `csr.write lr` when they need to adjust the resume PC.
- **Supervisor stack pointer (`SSP`)**: exposed through `CSR_SSP` and, during a trap, aliased onto `A7` so handlers can walk the privileged stack without disturbing the user-mode `A7`.
- **Capability tags (`A` bank sidecars)**: every architectural `A` register carries hidden base, limit, permission (R/W/X), and valid metadata. Loads and stores consult the sidecar before issuing memory requests; metadata resets to an all-permitted, full-range capability so existing bare-metal code keeps running until the supervisor tightens the window via the capability CSRs.

### Immediate Sign-Extension
- 27-bit immediates extend to 48 bits for ALU-immediate forms.
- 33-bit offsets cover load/store displacement and JALR immediates.
- 31-bit branch offsets and 36-bit JAL offsets are added to `PC+1`.

## Instruction Classes & Formats

| Class | Opcode[47:44] | Highlights |
| ----- | ------------- | ---------- |
| `ALU` | `0000`        | Register ALU ops with optional operand swap bit for commutatives. |
| `ALUI_A` | `0001`     | ALU immediate targeting either bank; source drawn from A-bank. |
| `ALUI_D` | `0010`     | ALU immediate with D-bank source. |
| `LD`  | `0011`        | D-bank destination; base from A-bank with optional post-increment. |
| `ST`  | `0100`        | Writes DMEM from D-bank; optional post-increment of base. |
| `BR`  | `0101`        | PC-relative conditional branches comparing A vs D registers. |
| `JAL` | `0110`        | Link to specified bank/register; target is `PC+1+off`. |
| `JALR`| `0111`        | Indirect jump via A-bank plus immediate; optional link. |
| `CSR` | `1000`        | Access machine CSRs (`csr.r`, `csr.rw`, `csr.rs`, `csr.rc`). |
| `SYS` | `1111`        | `funct=0` NOP, `funct=1` breakpoint exception, `funct=2` interrupt return (IRET), `funct=15` HALT, others illegal. |

### ALU Operations
- Implemented in `src/rtl/alu.v` with opcode nibble mapping to 6-bit `alu_op`.
- Supported ops: ADD, SUB, AND, OR, XOR, SLL, SRL, SRA, NOT.
- `swap` bit (bit 29 of ALU format) allows commutative instructions to treat `D` as the left operand, enabling shifts with variable shamt sourced from D.

### Memory Access
- Word-addressed: `disp33` and `off` values are counted in 48-bit words. Byte addressing is intentionally omitted.
- Post-increment applies after load/store for pointer walking; update suppressed when the base is `A0`.
- DMEM writes occur on the clock edge; reads are combinational per `simple_mem48`.
- Capability metadata attached to the base register must permit the requested access (`R` for loads, `W` for stores) and cover the computed address; otherwise a capability fault is raised before DMEM is touched.

### Control Flow Notes
- Branch conditions compare `A[rsA]` against `D[rsD]` using both signed and unsigned comparators.
- JAL/JALR links respect the `rdBank` bit; writes to `A0` are ignored by the register file.
- HALT stabilizes `PC` by forcing `next_pc = pc` while `halt` remains asserted.

### CSR Instructions
- The `csr` opcode class provides four primitives:
  - `csr.r rd, csr` — read the selected CSR into `rd` (source D register is ignored; use `d0`).
  - `csr.rw rd, csr, rs` — write the CSR with `rs` and optionally capture the old value in `rd` (`a0` discards it).
  - `csr.rs rd, csr, rs` — set bits (`CSR ← CSR ∨ rs`), returning the previous contents.
  - `csr.rc rd, csr, rs` — clear bits (`CSR ← CSR ∧ ¬rs`), returning the previous contents.
- Only D-bank registers may supply CSR operands; results can target either bank. The assembler also accepts friendly aliases (`csr.write`, `csr.set`, `csr.clear`, `csr.read`) that map onto the same encodings.
- Accesses are privilege-checked against the current `priv_mode`. Supervisor code (kernel mode) may read and write machine-control CSRs (`STATUS`, `SCRATCH`, `EPC`, `CAUSE`, the IRQ registers, and the timer block), while user mode is restricted to the performance counters (`CYCLE`/`INSTRET`). Any attempt made below the required privilege traps as an illegal instruction.
- Writing the `STATUS` CSR updates `priv_mode` to match bits `[1:0]` of the written value so the architectural and internal views stay coherent. Bits `[3:2]` hold the stacked privilege level that hardware captures on entry to a trap; `IRET` consumes it to determine the resume mode and then resets the field to user. Bits `[6:4]` provide per-mode interrupt enables (`UIE`/`KIE`/`MIE`), while bits `[9:7]` retain the previously-active values (`UPIE`/`KPIE`/`MPIE`) so handlers can restore state without manual bookkeeping. `CYCLE` increments every clock tick and `INSTRET` increments when `halt` is low; software may overwrite either counter to seed or reset them.
- Implemented CSRs:
  - `0x000 STATUS` — machine status flags; bits `[1:0]` mirror `priv_mode`, `[3:2]` reflect the stacked privilege captured by the most recent trap, `[6:4]` are the user/supervisor/machine interrupt enables, and `[9:7]` retain their previous values for `IRET`.
  - `0x001 SCRATCH` — general-purpose machine scratch register.
  - `0x002 EPC` — latched PC associated with the most recent trap.
  - `0x003 CAUSE` — trap cause; bit `47` is set for interrupts, while the low bits encode the exception/interrupt identifier (e.g. `2`=illegal instruction, `3`=breakpoint, `4`=load misaligned, `6`=store misaligned, interrupt IDs start at `0`).
  - `0x004 LR` — dedicated trap link register; always mirrors `EPC` and supplies the resume address consumed by `IRET`.
  - `0x005 SSP` — supervisor stack pointer value made visible to trap handlers and updated transparently when they manipulate `A7`.
  - `0x010 IRQ_ENABLE` — per-line interrupt mask (bit set enables the corresponding `irq` input).
  - `0x011 IRQ_PENDING` — latched interrupt requests; hardware sets/clears bits, and software can manipulate them via `csr.rs`/`csr.rc` for acknowledgement.
  - `0x012 IRQ_VECTOR` — base PC for interrupt handlers; the selected IRQ index is added to this value on entry.
  - `0x200-0x207 CAP_BASE<n>` — 48-bit base word address bounds for `A<n>` capabilities (`n` in `0..7`). Writes preserve limit/permissions and the valid flag.
  - `0x210-0x217 CAP_LIMIT<n>` — 48-bit inclusive high bounds for `A<n>` capabilities.
  - `0x220-0x227 CAP_PERM<n>` — capability control for `A<n>`; bits `[2:0]` hold the RWX permissions and bit `3` toggles the valid tag. Writes leave base/limit unchanged. Only supervisor/machine mode may touch these CSR ranges; user-mode attempts trap as illegal instructions.
  - `0xC00 CYCLE` — free-running cycle counter.
  - `0xC01 INSTRET` — retired-instruction counter.
  - `0xC02 TIMER` — machine timer counter that increments each cycle unless explicitly written.
  - `0xC03 TIMER_CMP` — timer compare register; when non-zero and `TIMER ≥ TIMER_CMP`, the timer interrupt line asserts until software updates `TIMER_CMP` or clears the pending bit.

### Traps, Exceptions & Interrupts
- Synchronous exceptions cover illegal instruction decodes, software breakpoints (`SYS 1`), misaligned data accesses (addresses that overflow/underflow the configured DMEM range), and capability violations (cause codes `8` for loads, `9` for stores) when a base register’s metadata is invalid or lacks the required permission. In addition, level-sensitive interrupt requests presented on the `irq[IRQ_LINES-1:0]` port are latched into `CSR_IRQ_PENDING` and serviced when the line’s enable bit is set _and_ the interrupt-enable bit for the current privilege level (`UIE`, `KIE`, or `MIE`) is high.
- When a trap occurs, register writes, DMEM writes, and CSR side-effects for the triggering instruction are suppressed. `csr_epc` captures the PC that execution should resume from and `csr_cause` records the exception code or interrupt index (with bit `47` set for interrupts).
- Trap entry simultaneously latches the resume address into the dedicated `LR` register, asserts the handler context so that `A7` accesses `SSP`, and keeps `CSR_EPC` mirrored to `LR`. Handlers can therefore read or modify the link value via the CSR interface, and manipulating `A7` while in the trap context updates the supervisor stack pointer without touching the user stack.
- Pending interrupt bits remain asserted until software clears them with `csr.clear` (or rewrites `CSR_IRQ_PENDING`), allowing simple handler acknowledgement schemes. The handler PC is computed as `IRQ_VECTOR + irq_id`, letting software install per-source entry points or a compact table of jumps.
- `csr_instret` does not increment for the trapped instruction, ensuring software-visible retirement counts stay precise.
- `SYS 0x2` (`iret`) consults the stacked privilege and interrupt-enable fields inside `STATUS`, restores `priv_mode`, re-applies the saved enable bit for the target mode, and jumps back to the value held in `LR` (which mirrors `CSR_EPC`). The register always reflects the instruction that would have executed next when the trap was taken, preserving precise sequencing for both exceptions and interrupts.
- Execution resumes at `TRAP_VECTOR` (default `64`) for exceptions and at `IRQ_VECTOR + irq_id` (default base `128`) for interrupts. Populate these addresses in IMEM with the appropriate handlers or halting sequences for your environment.

## Module Breakdown

- **`src/rtl/cpu_ad48.v`**: Top-level CPU tying together fetch, decode, execute, memory, and control logic. Parameters `IM_WORDS`/`DM_WORDS` set instruction/data memory depth; `TRAP_VECTOR` selects the synchronous exception handler PC (default `64`), while `IRQ_LINES` and `IRQ_VECTOR` configure the interrupt fabric. External interrupt requests enter via the `irq[IRQ_LINES-1:0]` port.
- **`src/rtl/cpu_ad48_instr.vh`**: Shared opcode constants and helper functions for assembling instructions (used by RTL and verification).
- **`src/rtl/alu.v`**: 48-bit ALU implementation with comparator outputs used for branch decisions.
- **`src/rtl/regfiles.v`**: Two 8x48 register banks with synchronous write ports; the A-bank carries capability sidecars (base/limit/perms/valid) that the core enforces on memory accesses.
- **`src/rtl/mem48.v`**: Synchronous write / combinational read memory; instantiated twice for IMEM and DMEM.
- **`src/rtl/simple_soc_stub.v`**: Convenience wrapper around the CPU for integration or `$readmemh` preload hooks.

## Reset, Clock, and Halt Semantics
- `resetn` is active-low and synchronously releases the PC and register files after four cycles in verification (no internal counters depend on the duration).
- On reset, all registers and memories initialize to zero via `initial` blocks.
- HALT is observable via `halt` in the CPU scope; exiting reset then executing `SYS` with `funct=0xF` will hold PC and maintain register state.

## Verification
- **`verif/env/cpu_ad48_alu_tb.v`**: Exercises ALU paths, swap behavior, and basic immediate handling. Confirms writes to `A0` are suppressed.
- **`verif/env/cpu_ad48_ctrl_tb.v`**: Covers branch conditions, JAL/JALR linking, loop control, and HALT behavior.
- **`verif/env/cpu_ad48_mem_tb.v`**: Validates load/store displacement, post-increment sequencing, and DMEM updates.
- **`verif/env/cpu_ad48_exc_tb.v`**: Checks synchronous exception entry, cause/EPC reporting, and precise retirement counts for illegal, breakpoint, and misaligned accesses.
- **`verif/env/cpu_ad48_irq_tb.v`**: Exercises interrupt masking, pending/enable CSRs, cause/EPC reporting, and `SYS 0x2` return sequencing.

Each testbench uses the instruction helper functions to compose programs directly into IMEM, then monitors register/memory state via hierarchical references. Running all three provides baseline functional confidence; waveform inspection is optional thanks to self-checking tasks.

## Integration & Extension Notes
- Adjust `IM_WORDS`/`DM_WORDS` parameters to match target memory budgets. `pc_idx` and DMEM addressing automatically size to the depth via `$clog2`.
- To connect external memories, replace `simple_mem48` with vendor-specific blocks or expose ports at the top level (consider updating the SoC stub).
- Additional instructions can reuse the existing decode scaffolding: extend `cpu_ad48_instr.vh`, map new opcodes in the `case (op)` statement, and route operands through the ALU or bespoke datapaths.
- For multi-cycle operations or stalls, introduce pipeline registers around the execute/memory section and gate `next_pc` updates accordingly; the single-cycle baseline simplifies this evolution.
