# Amber48 ISA Details

Below is a **clean BAU-native** that **removes all 8-bit and 16-bit notions**. The architecture exposes **only two data granularities**:

* **48-bit scalar** (the BAU itself), and
* **packed 2x24-bit lanes** inside a BAU (with both **`.u` modular** and **`.s` saturating** variants).

Everything-addresses, offsets, caches, pages-is expressed in **BAUs**.

This assumes a **clean-slate environment**: there are **no compatibility constraints** with legacy ISAs, operating systems, toolchains, language ABIs, RFCs, character encodings, floating-point formats, or other external standards. Every encoding and convention is free to prioritize BAU-native behavior.

### Assembler naming (lowercase only)

Amber48 adopts custom lowercase mnemonics instead of the usual assembly names.

- **Abbreviations:** `oper` (operation), `cond` (conditional), `ls` (logic shift), `as` (arithmetic shift), `rot` (rotate), `sub` (subroutine), `imm` (immediate).
- **Core instructions:** `no_oper`, `halt`, `copy`, `copy_cond`, `negate`, `subtract`, `add`, `not`, `and`, `or`, `xor`, `ls_left`, `ls_right`, `as_right`, `rot_left`, `rot_right`, `compare`, `is_zero`, `target_pad`, `branch`, `branch_cond`, `branch_sub`, `jump`, `jump_cond`, `jump_sub`, `return`, `push`, `pop`.
- **Immediate variants:** `add_imm`, `subtract_imm`, `xor_imm`, `compare_imm`, `upper_imm`.
- **Packed arithmetic & logical:** `pack_add`, `pack_subtract`, `pack_negate`, `pack_and`, `pack_or`, `pack_xor`, `pack_not` (each with `.u`/`.s` where defined).
- **Packed shifts & lane ops:** `pack_ls_left`, `pack_ls_right`, `pack_as_right`, `pack_rot_left`, `pack_rot_right`, `pack_extract`, `pack_insert`, `pack_compare`, optional `lane_ptr_inc`/`lane_ptr_dec`.
- **Branches & conditions:** `branch`, `branch_cond` (`eq`, `ne`, `lt.u`, `lt.s`, `gt.u`, `gt.s`, `z`, `nz`), `branch_sub`, `target_pad`, `jump`, `jump_cond`, `jump_sub`.
- **CSR/other system ops:** `csr_read`, `csr_write`, `csr_set`, `csr_clear`, `csr_swap`.
- **Security & traps:** `land`, `soft_interrupt`, `sys_call`, `sys_return`.
- **Capability/CHERI ops:** `cap_load`, `cap_store`, `cap_copy`, `cap_set_bounds`, `cap_set_offset`, `cap_seal`, `cap_unseal`, `cap_guard`, `cap_check`, `cap_branch`, `cap_branch_cond`.
- **Memory capability transfers:** `cap_load_pair`/`cap_store_pair` move 2-BAU capability images and preserve tags.

These names appear in opcode tables and documentation wherever an `isa` column is shown.

---

## BAU-48 Minimal Spec (v0.2, BAU-native only 48 & 24)

### Fixed Parameters

* **BAU (addressing unit):** 48 bits. Every architectural address selects one BAU.
* **Data granularities:** 48-bit scalar; **packed 2x24-bit** only.
* **Instruction widths:** 12 / 24 / 48 bits. 12/24 may be packed; **XT** stage unpacks between IF and ID.
* **Pages:** **2^12 BAU** (base) and **2^24 BAU** (large) pages.
* **Caches:** **I\$ and D\$ = 16 lines x 16 BAU/line** (direct-mapped baseline; growable later).
* **Short regs in 12-bit forms:** fixed **3-bit subset `r0..r7`**.
* **Pointers are CHERI-style capabilities** (96-bit).
* **Security:** LR/SSP + **12-bit `land`** (hardware flow-control protection).
* **ISA features:** 48-bit/24-bit arithmetic (no mul/div yet), logic & shifts/rotates, branches, compare/test, CSR, `jump_sub/return/push/pop` via micro-ops.
* **MMU:** 48-bit VA -> (<=42-bit PA), with capability checks in parallel.

#### enid I/O Fabric

* **Link family:** `enid` is a 48-bit, dual-simplex SerDes fabric with credit-based flow control and 12b/13b framing. It mirrors PCIe-style transaction ordering but carries BAU-sized payloads so every request and completion aligns with the native 48-bit width.
* **Root complex (3-port):**
  * **Port 0:** uplink into the enid switch/backplane that fans out to additional slots.
  * **Port 1:** left module bay (hot-swappable).
  * **Port 2:** right module bay (hot-swappable).
* **Module classes:** switch expansion sleds, discrete GPU, audio DSP, mass storage, human-interface, wired/wireless network, and NPU accelerators all plug into the same enid connector and capability hand-off.
* **Discovery:** each port exposes a capability-based configuration window (`config_cap`) describing vendor/device IDs, BAR layout, MSI vectors, and security policy. Firmware probes the three root ports, walks any downstream switch, and installs per-device capabilities for the OS.
* **Transactions:** memory reads/writes and doorbells carry 48-bit BAU addresses. DMA engines present capability tokens (issued by firmware) with explicit bounds, allowing the root complex to clamp errant masters before the request leaves the CPU tile.


---

### Memory Model (BAU-native)

* **Addresses index BAUs.** Offsets in load/store are in **BAUs**.
* **Packed sub-BAU access:** only **24-bit** slots inside one BAU:

  * `slot2 in {0,1}` -> lower lane `[23:0]`, upper lane `[47:24]`.
* **No byte/halfword concepts exist in the ISA.** Toolchains must lower smaller language types to 24-bit lanes (or widen to 48).

#### Misalignment & Atomicity (BAU-native)

* **48-bit BAU transfers (`load/store`)**: inherently aligned by construction (address selects one BAU).
* **Lane observation/updates:** memory never transfers single-lane quantities; software loads the containing BAU and uses packed-mask/shift ops to isolate or merge 24-bit lanes before writing back.
* **Atomicity:** BAU transfers are atomic with respect to a single BAU; there is **no cross-BAU atomicity**.

---

### Loads and Stores (BAU-wide only)

All addressing is `capability + integer_base + offBAU` with **offBAU** in BAUs. Every memory transfer moves exactly one BAU (48 bits); packed 24-bit lanes are handled via register operations after the load and before the store.

#### enid MMIO & DMA Semantics

* **Configuration:** each enumerated enid function exposes a 4-BAU control aperture via its `config_cap`; drivers discover BARs, MSI vectors, and doorbells entirely through capability loads/stores.
* **MMIO ordering:** CPU-issued MMIO writes toward enid endpoints are strongly ordered; the root complex posts them but only reports completion after the downstream link accepts the packet. Reads are non-posted and complete in request order per-function.
* **DMA addressing:** endpoints emit BAU-granular reads/writes tagged with their capability tokens. The root complex validates the token, clamps the transfer to the advertised bounds, and rewrites the BAU address into system physical space.
* **Doorbells:** BAU-wide doorbell writes (e.g., queue tail pointers) are translated into sideband messages so software never depends on sub-BAU semantics. Optional remote atomics are limited to whole-BAU compare-and-swap/add sequences when the device advertises support.
* **Isolation:** firmware may revoke or shrink a device token at runtime; subsequent DMA attempts fault as `CapFault` at the root complex and raise an interrupt for containment.

* **Implicit-cap (via `cDDC`):**

  * `load   rd, [ra + offBAU]`
  * `store   [ra + offBAU], rs`
* **Explicit-cap (use capability `rc`):**

  * `cap_load  rd, [rc + ra + offBAU]`
  * `cap_store  [rc + ra + offBAU], rs`

> **PC-relative** forms are available for BAU-aligned code/data (`PC + offBAU`). To work on a single 24-bit element, load the containing BAU, extract or update the target lane with packed shifts/masks (e.g., `pack_ls_right`, `pack_ls_left`, `pack_and`), then write the full BAU back with `store/cap_store`.

**Capability transfers:**

* `cap_load_pair rd, [rc + ra + offBAU]`
* `cap_store_pair [rc + ra + offBAU], rs`

These forms move **two consecutive BAUs** (lower then upper) in a single architectural operation and preserve the capability **tag**. The microarchitecture guarantees **atomic visibility**: observers either see the old capability image or the fully updated one, never a torn half. The instructions fault if either BAU fails bounds or permissions.

**Encoding sketch (representative):**

* Short 12/24-bit syllables provide compact offsets for `load/store`.
* 48-bit long forms provide larger `offBAU` or absolute addresses.

---

### Packed 24-bit Arithmetic (two lanes per BAU)

All packed ops operate on **two independent 24-bit lanes** inside each 48-bit register.

* **Modular (`.u`)**: wrap mod 2^24.
  Examples: `pack_add.u`, `pack_subtract.u`, `pack_negate.u`.
* **Saturating (`.s`)**: clamp to `+0x7FFFFF / -0x800000`.
  Examples: `pack_add.s`, `pack_subtract.s`, `pack_negate.s`.

**Flags (recommended):**

* Summary **N Z C V** reflect whole-register results (e.g., `Z=1` iff both lanes zero).
* Optionally, a **lane mask CSR** can expose per-lane overflow/saturation detail; in `.s`, **V=1** if *any* lane saturated.

**Logic & shifts (packed):**

* `pack_and`, `pack_or`, `pack_xor`, `pack_not`
* Shifts/rotates lane-local: `pack_ls_left`, `pack_ls_right`, `pack_as_right`, `pack_rot_left`, `pack_rot_right`
  Shift counts: immediate or `rb[5:0]`, masked **0..23** for lane ops.

**Lane moves & merge:**

* `pack_extract rd, rs, slot2` copies lane `slot2` of `rs` into the low 24 bits of `rd`, zeroing the upper lane.
* `pack_insert rd, rs, slot2` writes the low 24 bits of `rs` into lane `slot2` of `rd` while leaving the other lane untouched.

`slot2` reuses the one-bit lane selector encoding (immediate 0/1 or via a register bit).

---

### Scalar 48-bit Arithmetic & Logic

* `add`, `add_imm`, `subtract`, `subtract_imm`, `negate`
* `and`, `or`, `xor`, `not` (48-bit)
* Shifts/rotates: `ls_left`, `ls_right`, `as_right`, `rot_left`, `rot_right` (mask counts to **0..47**)

> **mul/div** opcodes are reserved; later implemented via async unit (queue + completion).

---

### Compare, Test, and Branches

* `compare` / `compare_imm` update **NZCV** (signed/unsigned branches interpret flags accordingly).
* Packed compares (optional): `pack_compare.u`, `pack_compare.s` update summary flags + optional lane mask CSR.
* **Branch forms:**
  * `branch target` is an unconditional PC-relative jump.
  * `branch_cond cond, rs, target` tests `rs` against zero (conditions `z`/`nz`).
  * `branch_cond cond, rs, rt, target` compares two registers; `lt.s/lt.u/gt.s/gt.u/eq/ne` select signed or unsigned interpretation.
  * `branch_sub target` pushes a return address before branching.
* **Syllable encoding:** PC-relative displacements live in 12-bit syllables (not BAUs). Short (12-bit), medium (24-bit), and long (48-bit absolute) forms exist for control flow.
* **`jump_sub`/`return`** execute via micro-ops with **LR/SSP** and **HFCP** (`land tok9`) checks at indirect targets.

---

### Immediates: `upper_imm` + UI Latch (BAU semantics)

* **`upper_imm rd, imm24`** writes `rd[47:24]=imm24`, clears `rd[23:0]`.
* **UI latch**: holds a 24-bit **upper** fragment; any `.ext` form concatenates `UI:immN` -> wide immediates for addresses/data **in BAUs**; then clears UI.valid.
  Patterns: `upper_imm` + `add_imm.ext`, `upper_imm` + `load.ext`, etc.

---

