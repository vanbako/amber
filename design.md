Below is a **clean BAU‑native** that **removes all 8‑bit and 16‑bit notions**. The architecture exposes **only two data granularities**:

* **48‑bit scalar** (the BAU itself), and
* **packed 2×24‑bit lanes** inside a BAU (with both **`.u` modular** and **`.s` saturating** variants).

Everything—addresses, offsets, caches, pages—is expressed in **BAUs**.

This assumes a **clean-slate environment**: there are **no compatibility constraints** with legacy ISAs, operating systems, toolchains, language ABIs, RFCs, character encodings, floating-point formats, or other external standards. Every encoding and convention is free to prioritize BAU-native behavior.

### Assembler naming (lowercase only)

Amber48 adopts custom lowercase mnemonics instead of the usual assembly names.

- **Abbreviations:** `oper` (operation), `cond` (conditional), `ls` (logic shift), `as` (arithmetic shift), `rot` (rotate), `sub` (subroutine), `imm` (immediate).
- **Core instructions:** `no_oper`, `halt`, `copy`, `copy_cond`, `negate`, `subtract`, `add`, `not`, `and`, `or`, `xor`, `ls_left`, `ls_right`, `as_right`, `rot_left`, `rot_right`, `compare`, `is_zero`, `target_pad`, `branch`, `branch_cond`, `branch_sub`, `jump`, `jump_cond`, `jump_sub`, `return`, `sys_call`, `sys_return`, `upper_imm`, `load`, `store`, `push`, `pop`.
- **Suffixes:** `s` marks signed behaviour (for example, `add.s`, `subtract.s`).
- **CSR/other system ops:** `csr_read`, `csr_write`, `csr_set`, `csr_clear`, `csr_swap`.
- **Capability/CHERI ops:** `cap_load`, `cap_store`, `cap_copy`, `cap_set_bounds`, `cap_set_offset`, `cap_seal`, `cap_unseal`, `cap_guard`, `cap_check`, `cap_branch`, `cap_branch_cond`.
- **Memory capability transfers:** `cap_load_pair`/`cap_store_pair` name the 2‑BAU `cldcap`/`cstcap` forms.

These names appear in opcode tables and documentation wherever an `isa` column is shown.

---

# BAU‑48 Minimal Spec (v0.2, BAU‑native only 48 & 24)

## 0) Fixed parameters you requested

* **BAU (addressing unit):** 48 bits. Every architectural address selects one BAU.
* **Data granularities:** 48‑bit scalar; **packed 2×24‑bit** only.
* **Instruction widths:** 12 / 24 / 48 bits. 12/24 may be packed; **XT** stage unpacks between IF and ID.
* **Pages:** **2^12 BAU** (base) and **2^24 BAU** (large) pages.
* **Caches:** **I\$ and D\$ = 16 lines × 16 BAU/line** (direct‑mapped baseline; growable later).
* **Short regs in 12‑bit forms:** fixed **3‑bit subset `r0..r7`**.
* **Pointers are CHERI‑style capabilities** (96‑bit).
* **Security:** LR/SSP + **12‑bit `land`** (hardware flow‑control protection).
* **ISA features:** 48‑bit/24‑bit arithmetic (no mul/div yet), logic & shifts/rotates, branches, compare/test, CSR, `jsr/ret/push/pop` via µops.
* **MMU:** 48‑bit VA → (≤42‑bit PA), with capability checks in parallel.

---

## 1) Memory model (BAU‑native)

* **Addresses index BAUs.** Offsets in load/store are in **BAUs**.
* **Packed sub‑BAU access:** only **24‑bit** slots inside one BAU:

  * `slot2 ∈ {0,1}` → lower lane `[23:0]`, upper lane `[47:24]`.
* **No byte/halfword concepts exist in the ISA.** Toolchains must lower smaller language types to 24‑bit lanes (or widen to 48).

### Misalignment & atomicity (BAU‑native)

* **48-bit BAU transfers (`ld48/st48`)**: inherently aligned by construction (address selects one BAU).
* **Lane observation/updates:** memory never transfers single-lane quantities; software loads the containing BAU and uses packed-mask/shift ops to isolate or merge 24-bit lanes before writing back.
* **Atomicity:** BAU transfers are atomic with respect to a single BAU; there is **no cross-BAU atomicity**.

---

## 2) Loads/Stores (BAU-wide only)

All addressing is `capability + integer_base + offBAU` with **offBAU** in BAUs. Every memory transfer moves exactly one BAU (48 bits); packed 24-bit lanes are handled via register operations after the load and before the store.

* **Implicit-cap (via `cDDC`):**

  * `ld48   rd, [ra + offBAU]`
  * `st48   [ra + offBAU], rs`
* **Explicit-cap (use capability `rc`):**

  * `cld48  rd, [rc + ra + offBAU]`
  * `cst48  [rc + ra + offBAU], rs`

> **PC-relative** forms are available for BAU-aligned code/data (`PC + offBAU`). To work on a single 24-bit element, load the containing BAU, extract or update the target lane with packed shifts/masks (e.g., `pshr24`, `pshl24`, `pand24`), then write the full BAU back with `st48/cst48`.

**Capability transfers:**

* `cldcap rd, [rc + ra + offBAU]`
* `cstcap [rc + ra + offBAU], rs`

These forms move **two consecutive BAUs** (lower then upper) in a single architectural operation and preserve the capability **tag**. The microarchitecture guarantees **atomic visibility**: observers either see the old capability image or the fully updated one, never a torn half. The instructions fault if either BAU fails bounds or permissions.

**Encoding sketch (representative):**

* Short 12/24-bit syllables provide compact offsets for `ld48/st48`.
* 48-bit long forms provide larger `offBAU` or absolute addresses.

---

## 3) Packed 24‑bit arithmetic (two lanes per BAU)

All packed ops operate on **two independent 24‑bit lanes** inside each 48‑bit register.

* **Modular (`.u`)**: wrap mod 2^24.
  Examples: `padd24.u`, `psub24.u`, `pneg24.u`.
* **Saturating (`.s`)**: clamp to `+0x7FFFFF / -0x800000`.
  Examples: `padd24.s`, `psub24.s`, `pneg24.s`.

**Flags (recommended):**

* Summary **N Z C V** reflect whole‑register results (e.g., `Z=1` iff both lanes zero).
* Optionally, a **lane mask CSR** can expose per‑lane overflow/saturation detail; in `.s`, **V=1** if *any* lane saturated.

**Logic & shifts (packed):**

* `pand24`, `por24`, `pxor24`, `pnot24`
* Shifts/rotates lane‑local: `pshl24`, `pshr24`, `psar24`, `prol24`, `pror24`
  Shift counts: immediate or `rb[5:0]`, masked **0..23** for lane ops.

**Lane moves & merge:**

* `pextr24 rd, rs, slot2` copies lane `slot2` of `rs` into the low 24 bits of `rd`, zeroing the upper lane.
* `pins24 rd, rs, slot2` writes the low 24 bits of `rs` into lane `slot2` of `rd` while leaving the other lane untouched.

`slot2` reuses the one-bit lane selector encoding (immediate 0/1 or via a register bit).

---

## 4) Scalar 48‑bit arithmetic & logic

* `add48`, `addi48`, `sub48`, `subi48`, `neg48`
* `and/or/xor/not` (48‑bit)
* Shifts/rotates: `shl/shr/sar/rol/ror` (mask counts to **0..47**)

> **mul/div** opcodes are reserved; later implemented via async unit (queue + completion).

---

## 5) Compare, test, and branches

* `cmp48` / `cmpi48` update **NZCV** (signed/unsigned branches interpret flags accordingly).
* Packed compares (optional): `pcmp24.u`, `pcmp24.s` update summary flags + optional lane mask CSR.
* **Branches:** PC‑relative in **12‑bit syllables** (not BAUs). Short (12‑bit), medium (24‑bit), and long (48‑bit absolute).
* **`jsr`/`ret`** via µops with **LR/SSP** and **HFCP** (`land tok9`) checks at indirect targets.

---

## 6) Immediates: `lui` + UI latch (BAU semantics)

* **`lui rd, imm24`** writes `rd[47:24]=imm24`, clears `rd[23:0]`.
* **UI latch**: holds a 24‑bit **upper** fragment; any `.ext` form concatenates `UI:immN` → wide immediates for addresses/data **in BAUs**; then clears UI.valid.
  Patterns: `lui` + `addi48.ext`, `lui` + `ld48.ext`, etc.

---

## 7) Capability model (96‑bit, 2‑BAU storage)

* Conceptual fields: `base`, `length`, `perms`, `sealed`, `otype`, **tag**.
* **`c0=cPCC`** (code cap), **`c1=cDDC`** (default data cap).
* Loads/stores check `(BAU_addr ∈ [base, base+length))` and required perms.
* Representative cap ops (48-bit forms): `cmove`, `csetb`/`csetl`, `candp` (perm and), `cseal`/`cunseal`, `ccheck`.

`cstcap/cldcap` consume/produce two consecutive BAUs. The lower BAU carries bits `[47:0]`, the upper BAU carries bits `[95:48]` plus metadata, and the architectural tag is set only when both halves succeed. If either half faults, the store leaves memory untouched and the load terminates with the destination register unmodified.

**Storage & ABI:** a capability occupies **2 BAUs** in memory; `cst*`/`cld*` preserve the **tag**.

---

## 8) CSRs (selected 48‑bit)

* `STATUS`: mode (U/K), IE, **NZCV**, HFCP enable, ASID, CoreID
* `EPC`, `CAUSE`, `BADVADDR`
* `PTBR`, `TLBCFG`
* `ICFG`, `DCFG`
* `IVEC` (interrupt vector table capability)
* `UI` (upper‑immediate latch)
* `KEY0` (HFCP)
* `CPUID`, `TIMER`

`csrrd/csrwr/csrs/csrc` exist in 12/24/48‑bit forms (subset in 12‑bit).

---

## 9) MMU & pages (2^12 BAU, 2^24 BAU)

* **VA decomposition (base page):**
  `VA = [VPN2 11:0 | VPN1 11:0 | VPN0 11:0 | BAU_off 11:0]`
* **Large pages:** set size bit at level‑1 PTE → use **BAU\_off\[23:0]** and skip last lookup.
* **TLB:** ASID‑tagged; entry records page size.
* **Permissions:** R/W/X/U; capability checks happen after translation.
* **Traps:** PageFaultI/D, PrivFault, AlignFault, CapFault, CFault (HFCP), Illegal, Syscall.

---

## 10) Interrupts (hardware & software)

### Hardware interrupts

* Each core exposes up to 16 hardware lines; the interrupt fabric delivers the highest-priority pending line as a vector in `CAUSE[7:0]` with `CAUSE[47]=1`.
* Entry follows the standard trap path: pipeline drains, `EPC` latches the syllable-aligned return PC, and `STATUS.IE` is cleared until the handler issues `eret`.
* Vector dispatch reads `IVEC` (capability CSR for the interrupt table); each vector entry stores a sealed handler capability so ordinary capability checks still apply on entry.
* Nested hardware interrupts are allowed when the handler sets `STATUS.IE=1` after saving live state; priority resolution is strictly descending by line index (0 = highest).
* `TIMER` and platform GPIO sources wire in through the same mechanism; additional SoC devices extend the vector space without architecturally visible changes.

### Software interrupts

* `sint imm4` (12-bit form) raises an interrupt using cause code `0x10 | imm4`, sharing the hardware entry machinery (`CAUSE[47]=1`). The instruction is privileged; user mode traps with `PrivFault`.
* Software interrupts are intended for kernel-originated IPIs (e.g., scheduler ticks, cross-core shootdowns) and for debugging hooks. They do not bypass capability checks.
* **Syscall is *not* implemented via software interrupts.** A dedicated `syscall` µop raises synchronous trap cause `Syscall` (without setting `CAUSE[47]`), keeping the ABI stable even if software interrupts are disabled.
* Handlers return with `eret`, re-enabling interrupts according to the saved `STATUS` image.

---

## 11) Caches (16×16 BAU each)

* **I\$:** 16 lines × 16 BAU/line (**256 BAU** total). Direct‑mapped MVP.
  Fetch delivers BAU‑aligned windows to **XT**; `cPCC` bounds and X‑perm checked.
* **D\$:** 16 lines × 16 BAU/line (**256 BAU** total). Direct‑mapped MVP, write‑through (write‑back later).
  Capability and MMU perms enforced at **MF**.

> Associativity/size are fields in `ICFG/DCFG` so you can scale later without ABI changes.

---

## 12) Pipeline & timing (unchanged)

```
IA → IF → XT → ID → EX → MA → MF → WB
```

* **Single‑cycle** µops from ID→WB on cache hits.
* Misses stall MA/MF.
* XT enforces packing rules: **branch/indirect targets land on syllable 0** of a bundle.
* µops/ISA split enables `jsr/ret/push/pop` without multi-cycle instructions.

### Hazard considerations

- **Syllable alignment guard:** variable 12/24/48-bit syllables demand that every control-flow target land on bundle slot 0; a malformed encoder or bad patch will leave IF/XT misaligned until the pipe flushes.
- **Redirect latency:** branches resolve in XT and hardware traps drain the full IA→WB pipe, so each redirect inserts bubbles and mishandled `STATUS.IE` updates can reopen the window before state is saved.
- **Direct-mapped cache conflicts:** the 16×16-BAU I$ and D$ are single-way; competing addresses thrash a line and hold MA/MF stalled, back-pressuring earlier stages.
- **Packed lane read-modify-write:** memory only moves whole BAUs, forcing `cld/modify/cst`; pre-emption or another hart between the load and store can clobber the untouched lane.
- **Multi-BAU capability spills:** capabilities occupy two BAUs in memory and on the ABI stack, so faults between halves risk exposing a torn capability image.
- **Upper-immediate latch misuse:** any `.ext` consumer relies on a fresh `lui`; failing to clear or refill the UI latch silently builds the wrong wide immediate without a validity trap.
- **Packed saturation visibility:** `.s` packed ops only report summary NZCV flags unless the optional lane mask CSR is implemented, hiding per-lane overflow from software that expects it.
- **TLB size-bit aliasing:** large-page entries bypass the final level; inconsistent size bits or stale ASIDs can hand different permission sets to the same BAU, generating capability or privilege faults at MA/MF.

---

## 13) ABI (pointers as capabilities)

### Registers

* **Integers (48‑bit):** `r0=0` (hard‑zero); args **`r1..r4`**; ret **`r1`**; caller‑saved `r1..r5`; callee‑saved `r6..r12`; `r13=GP`, `r14=SP`, `r15=LR`.
* **Capabilities (96‑bit):**

  * `c0=cPCC` (reserved), `c1=cDDC` (reserved).
  * **Pointer args/ret:** `c2..c5` = **`ca0..ca3`**; return in `c2`.
  * Caller‑saved: `c6`; callee‑saved: `c7`.
  * Subroutine calls pass capability operands in `ca0..ca3`; the caller spills additional capability arguments into 2‑BAU stack slots using `cstcap`, and the callee reloads them with `cldcap`.

### Stack & spills

* `SP` is an **integer BAU address** into a stack region covered by `cDDC` (and optionally a tighter per‑thread stack cap in `c7`).
* **Capability spill slot = 2 BAUs**, aligned to 2‑BAU boundaries; only `cstcap/cldcap` preserve tags across the spill.
* **Integer spill slot = 1 BAU**.
* Varargs: excess **capability** args spill in 2‑BAU slots; **integer** args spill in 1‑BAU slots, in call order.

Callers preserve callee‑saved capability registers (`c7`) with `cstcap` before the call and restore them after return. Indirect calls load the target from a capability register; software must ensure the capability remains executable and unsealed so the hardware checks succeed when `jsr` transfers control.

### Using pointer args

* **48‑bit element:** `cld48 rd, [c2 + rX + offBAU]`
* **24-bit lane:** load the containing BAU with `cld48`, then isolate the desired lane via `pextr24` (or shifts/masks). To store, update the lane with `pins24` and emit a full-width `cst48`.

> Toolchain tip: a pointer to 24‑bit elements is represented as **(cap BAU‑ptr, lane slot)**. Increment steps **slot → BAU+slot wrap** (see below).

---

## 14) Lane‑aware pointer math (24‑bit arrays)

Represent a 24‑bit element pointer as `(cap, BAU_index, slot2)`.

* **Increment by 1 element:**

  * `slot2 ^= 1`
  * if `slot2 == 0` after toggle → `BAU_index += 1`
* **Decrement** mirrors the above.

Compilers can synthesize this with a couple of integer ops; optional helper opcodes (e.g., `inc24p/dec24p`) can be added later without ABI impact.

---

## 15) Example snippets

### 15.1 Fill 24-bit array (pointer as capability in `c2`)

```asm
; void fill24(u24_ptr p, u48 n, u24 v)
; p in c2 (ca0), n in r1, v in r2 (low 24 bits used)

  xor     r3, r3, r3        ; BAU index = 0
  xor     r4, r4, r4        ; slot2 = 0
L0:
  cld48   r5, [c2 + r3 + 0]       ; read BAU containing current element
  pins24  r5, r2, r4              ; overwrite lane slot2 with v (low 24 bits of r2)
  cst48   [c2 + r3 + 0], r5       ; write back the updated BAU
  xori    r4, r4, 1               ; slot ^= 1
  bnez    r4, SAME_BAU
  addi48  r3, r3, 1               ; wrapped -> next BAU
SAME_BAU:
  subi48  r1, r1, 1
  bne     L0
  ret
land 0x41
```

### 15.2 Mixed 48‑bit sum with packed add

```asm
; r1 = cap ptr to BAU array (c2), r2 = count (BAUs)
  xor     r3, r3, r3            ; sum48 = 0
SUM:
  cld48   r4, [c2 + 0 + 0]
  add48   r3, r3, r4
  addi48  r1, r1, 1             ; advance by 1 BAU
  subi48  r2, r2, 1
  bne     SUM
  ret
land 0x12
```

---

## 16) Minimal implementation checklist (48 & 24 only)

* [ ] 16×48 GPRs (`r0..r15`), `r0=0`; `SP`, `LR`, `SSP`.
* [ ] 8×96 capability regs; implicit `cDDC` and `cPCC`.
* [ ] **Only** BAU-wide loads/stores (`ld48/st48` + capability variants); **no 8/16/24-bit** anywhere.
* [ ] 12/24/48 decode + **XT** unpack; branch target rule; **`land`**.
* [ ] I\$ & D\$: **16×16 BAU** lines; direct‑mapped; cap/MMU checks at MF.
* [ ] MMU: 3‑level walk (12+12+12) with **2^12 BAU** pages; **2^24 BAU** large pages at level‑1.
* [ ] Scalar 48‑bit ALU + packed 24‑bit ALU (`.u` & `.s`).
* [ ] CSR access; `lui` + UI latch for wide immediates.
* [ ] Traps/IRQs; `jsr/ret/push/pop` via µops; HFCP token check.

---
