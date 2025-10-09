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
- **System**: HALT holds the PC and raises an internal `halt` flag observed by testbenches. Non-HALT SYS encodings behave as NOP today.
- **CSR file**: Status, scratch, EPC/CAUSE, and counter CSRs live alongside the datapath. CSR instructions apply privilege checks, update counters automatically, and keep the architectural `STATUS` view in sync with the internal `priv_mode`.

Timing remains single-cycle: all combinational work completes between clock edges, and the registered PC advances every cycle except during reset or HALT. There is no pipeline and thus no hazard management logic.

## Programming Model

### Registers
- **A-bank** (`A0`..`A7`): intended for addresses/base pointers. `A0` is hard-wired to zero; post-increment is disabled when `baseA == A0` to keep the invariant.
- **D-bank** (`D0`..`D7`): general-purpose data registers. Loads always write to this bank; stores read from it.
- **PC**: 48-bit word index. `PC+1` denotes the next sequential instruction.

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
| `SYS` | `1111`        | `funct=0` NOP, `funct=15` HALT (holds PC). |

### ALU Operations
- Implemented in `src/rtl/alu.v` with opcode nibble mapping to 6-bit `alu_op`.
- Supported ops: ADD, SUB, AND, OR, XOR, SLL, SRL, SRA, NOT.
- `swap` bit (bit 29 of ALU format) allows commutative instructions to treat `D` as the left operand, enabling shifts with variable shamt sourced from D.

### Memory Access
- Word-addressed: `disp33` and `off` values are counted in 48-bit words. Byte addressing is intentionally omitted.
- Post-increment applies after load/store for pointer walking; update suppressed when the base is `A0`.
- DMEM writes occur on the clock edge; reads are combinational per `simple_mem48`.

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
- Accesses are privilege-checked against the current `priv_mode`. Machine-only CSRs (status, scratch, EPC, cause) reject reads/writes once privilege is lowered, while the cycle and instret counters are readable from all modes.
- Writing the `STATUS` CSR updates `priv_mode` to match bits `[1:0]` of the written value so the architectural view and internal mode stay coherent. `CYCLE` increments every clock tick and `INSTRET` increments when `halt` is low; software may overwrite either counter to seed or reset them.
- Implemented CSRs:
  - `0x000 STATUS` — machine status flags; bits `[1:0]` mirror `priv_mode`.
  - `0x001 SCRATCH` — general-purpose machine scratch register.
  - `0x002 EPC` and `0x003 CAUSE` — placeholders for future trap plumbing.
  - `0xC00 CYCLE` — free-running cycle counter.
  - `0xC01 INSTRET` — retired-instruction counter.

## Module Breakdown

- **`src/rtl/cpu_ad48.v`**: Top-level CPU tying together fetch, decode, execute, memory, and control logic. Parameters `IM_WORDS`/`DM_WORDS` set instruction/data memory depth.
- **`src/rtl/cpu_ad48_instr.vh`**: Shared opcode constants and helper functions for assembling instructions (used by RTL and verification).
- **`src/rtl/alu.v`**: 48-bit ALU implementation with comparator outputs used for branch decisions.
- **`src/rtl/regfiles.v`**: Two 8x48 register banks with synchronous write ports.
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

Each testbench uses the instruction helper functions to compose programs directly into IMEM, then monitors register/memory state via hierarchical references. Running all three provides baseline functional confidence; waveform inspection is optional thanks to self-checking tasks.

## Integration & Extension Notes
- Adjust `IM_WORDS`/`DM_WORDS` parameters to match target memory budgets. `pc_idx` and DMEM addressing automatically size to the depth via `$clog2`.
- To connect external memories, replace `simple_mem48` with vendor-specific blocks or expose ports at the top level (consider updating the SoC stub).
- Additional instructions can reuse the existing decode scaffolding: extend `cpu_ad48_instr.vh`, map new opcodes in the `case (op)` statement, and route operands through the ALU or bespoke datapaths.
- For multi-cycle operations or stalls, introduce pipeline registers around the execute/memory section and gate `next_pc` updates accordingly; the single-cycle baseline simplifies this evolution.
