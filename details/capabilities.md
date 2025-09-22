# Capability Model Details

## Architectural Capabilities

* Conceptual fields: `base`, `length`, `perms`, `sealed`, `otype`, **tag**.
* **`c0=cPCC`** (code cap), **`c1=cDDC`** (default data cap).
* Loads/stores check `(BAU_addr in [base, base+length))` and required perms.
* Representative cap ops (48-bit forms): `cap_copy`, `cap_set_bounds`, `cap_set_offset`, `cap_guard`, `cap_seal`, `cap_unseal`, `cap_check`.

`cap_store_pair/cap_load_pair` consume/produce two consecutive BAUs. The lower BAU carries bits `[47:0]`, the upper BAU carries bits `[95:48]` plus metadata, and the architectural tag is set only when both halves succeed. If either half faults, the store leaves memory untouched and the load terminates with the destination register unmodified.

**Storage & ABI:** a capability occupies **2 BAUs** in memory; only `cap_store_pair/cap_load_pair` preserve the **tag**.

## Lane-Aware Pointer Math (24-bit arrays)

Represent a 24-bit element pointer as `(cap, BAU_index, slot2)`.

* **Increment by 1 element:**
  * `slot2 ^= 1`
  * if `slot2 == 0` after toggle -> `BAU_index += 1`
* **Decrement** mirrors the above.

Compilers can synthesize this with a couple of integer ops; optional helper opcodes (e.g., `lane_ptr_inc`/`lane_ptr_dec`) can be added later without ABI impact.

