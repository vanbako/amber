# ABI and Software Model Details

## Register and Call Conventions

* **Integers (48-bit):** `r0=0` (hard-zero); args `r1..r4`; return `r1`; caller-saved `r1..r5`; callee-saved `r6..r12`; `r13=GP`, `r14=SP`, `r15=LR`.
* **Capabilities (96-bit):**
  * `c0=cPCC` (reserved), `c1=cDDC` (reserved).
  * Pointer args/return use `c2..c5` (`ca0..ca3`); return in `c2`.
  * Caller-saved: `c6`; callee-saved: `c7`.
  * Additional capability arguments spill to 2-BAU stack slots via `cap_store_pair`; callees reload with `cap_load_pair`.

## Stack & Spill Management

* `SP` is an integer BAU address inside a region covered by `cDDC` (optionally narrowed by a thread-local stack cap in `c7`).
* Capability spill slot = 2 BAUs aligned to 2-BAU boundaries; only `cap_store_pair/cap_load_pair` preserve tags across the spill.
* Integer spill slot = 1 BAU.
* Varargs: excess capability args spill in 2-BAU slots; integer args spill in 1-BAU slots in call order.

Callers preserve callee-saved capability registers (`c7`) with `cap_store_pair` before the call and restore them after return. Indirect calls consume executable, unsealed capabilities; `jump_sub` performs the hardware checks when transferring control.

## Using Pointer Arguments

* 48-bit element: `cap_load rd, [c2 + rX + offBAU]`.
* 24-bit lane: load the containing BAU with `cap_load`, isolate the lane via `pack_extract` (or shifts/masks), update with `pack_insert`, then commit with `cap_store`.

Lane-select increments for packed 24-bit arrays are described in `details/capabilities.md`.

## Example Snippets

### Fill 24-bit Array

```asm
; void fill24(u24_ptr p, u48 n, u24 v)
; p in c2 (ca0), n in r1, v in r2 (low 24 bits used)

  xor       r3, r3, r3
  xor       r4, r4, r4
L0:
  cap_load  r5, [c2 + r3 + 0]
  pack_insert r5, r2, r4
  cap_store [c2 + r3 + 0], r5
  xor_imm   r4, r4, 1
  branch_cond nz, r4, SAME_BAU
  add_imm   r3, r3, 1
SAME_BAU:
  subtract_imm r1, r1, 1
  branch_cond nz, r1, L0
  return
land 0x41
```

### Mixed 48-bit Sum with Packed Add

```asm
; c2 = capability pointer to BAU array, r1 = remaining count (BAUs)
  xor       r3, r3, r3
  xor       r4, r4, r4
SUM:
  cap_load  r5, [c2 + r4 + 0]
  add       r3, r3, r5
  add_imm   r4, r4, 1
  subtract_imm r1, r1, 1
  branch_cond nz, r1, SUM
  return
land 0x12
```

