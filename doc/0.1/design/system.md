# System Architecture Details

## Control and Status Registers (CSRs)

* `STATUS`: mode (U/K), IE, **NZCV**, HFCP enable, ASID, CoreID
* `EPC`, `CAUSE`, `BADVADDR`
* `PTBR`, `TLBCFG`
* `ICFG`, `DCFG`
* `DMACTL` (`sync_dma` fence bit, enid flow-control status)
* `IVEC` (interrupt vector table capability)
* `UI` (upper-immediate latch)
* `KEY0` (HFCP)
* `CPUID`, `TIMER`

`csr_read/csr_write/csr_set/csr_clear` exist in 12/24/48-bit forms (subset in 12-bit).

---

## MMU & Pages (2^12 BAU, 2^24 BAU)

* **VA decomposition (base page):**
  `VA = [VPN2 11:0 | VPN1 11:0 | VPN0 11:0 | BAU_off 11:0]`
* **Large pages:** set size bit at level-1 PTE -> use **BAU\_off\[23:0]** and skip last lookup.
* **TLB:** ASID-tagged; entry records page size.
* **Permissions:** R/W/X/U; capability checks happen after translation.

### enid address map & capability windows

* **System DRAM:** `BAU[0x0000_0000_0000 .. 0x000F_FFFF_FFFF]` remains cacheable and shareable across CPU harts and coherent accelerators.
* **enid RC configuration:** `BAU[0xFFF0_0000_0000 .. 0xFFF0_0000_0FFF]` maps the three root-complex capability windows (`config_cap[0..2]`). Access is non-cacheable and requires privileged capability permissions.
* **enid doorbells & queues:** `BAU[0xFFF0_0001_0000 .. 0xFFF0_000F_FFFF]` provides per-device, per-queue BAU slots that drivers program as MMIO doorbells; firmware allocates disjoint ranges when enumerating the fabric.
* **Hot-plug spares:** `BAU[0xFFF0_0010_0000 .. 0xFFF0_001F_FFFF]` is reserved so late-bound modules can map BARs without relocating existing devices.
* **Capability hand-off:** each discovered endpoint receives a sealed data capability pointing at its MMIO window plus (optionally) a second capability for inbound DMA buffers; software never hands out raw physical BAUs.

---

## Interrupts (Hardware & Software)

### Hardware interrupts

* Each core exposes up to 16 hardware lines; the interrupt fabric delivers the highest-priority pending line as a vector in `CAUSE[7:0]` with `CAUSE[47]=1`.
* Entry follows the standard trap path: pipeline drains, `EPC` latches the syllable-aligned return PC, and `STATUS.IE` is cleared until the handler issues `sys_return`.
* Vector dispatch reads `IVEC` (capability CSR for the interrupt table); each vector entry stores a sealed handler capability so ordinary capability checks still apply on entry.
* Nested hardware interrupts are allowed when the handler sets `STATUS.IE=1` after saving live state; priority resolution is strictly descending by line index (0 = highest).
* **enid MSI bridge:** enid endpoints raise interrupts by issuing 48-bit MSI messages. The root complex translates each message into a core hardware line, populates `CAUSE[15:8]` with the requester ID, and can steer vectors per-port for isolation. Firmware programs the MSI table during enumeration.
* `TIMER` and platform GPIO sources wire in through the same mechanism; additional SoC devices extend the vector space without architecturally visible changes.

### Software interrupts

* `soft_interrupt imm4` (12-bit form) raises an interrupt using cause code `0x10 | imm4`, sharing the hardware entry machinery (`CAUSE[47]=1`). The instruction is privileged; user mode traps with `PrivFault`.
* Software interrupts are intended for kernel-originated IPIs (e.g., scheduler ticks, cross-core shootdowns) and for debugging hooks. They do not bypass capability checks.
* **Syscall is *not* implemented via software interrupts.** A dedicated `sys_call` path (see `details/micro-ops.md` for uppercase micro-ops) raises synchronous trap cause `Syscall` (without setting `CAUSE[47]`), keeping the ABI stable even if software interrupts are disabled.
* Handlers return with `sys_return`, re-enabling interrupts according to the saved `STATUS` image.

---

## Caches and Coherence

* **I$:** 32 KB, 4-way, 16-byte sectors. Fetch delivers BAU-aligned bundles to **XT**; execute permissions are checked against the capability tag cached with each line.
* **D$:** 32 KB, 4-way, write-back with 16-byte sectors. Capability and MMU permissions are enforced at **MF**; stores stage capability metadata so writebacks carry the correct R/W/X/TRACE/LOAN summary into the fabric.
* **Amber Coherence Fabric (ACF):**
  - Core request channel emits `ReqRd` for shared fills and `ReqRdx` for exclusive ownership. Upgrades are triggered when a short-form window or capability store needs write permission.
  - The 256 KB shared slice acts as the home agent. Each directory entry tracks a four-bit sharer vector plus the capability permission summary, and returns `RespData` with the current metadata.
  - The snoop channel issues `SnpInv` when a line must be invalidated and `SnpClean` when the home agent needs a clean copy. Capability permission tightenings or LOAN expirations are treated as snoop events so L1s reconcile metadata before data is observed.
  - External masters attach through the same ACF request/response/snoop message set. Transactions flagged non-coherent use the `NC` request type; they bypass the slice and rely on software fences (`capability_fence prefix`, `DMACTL.sync_dma`).
* **Prefix fencing:** `capability_fence prefix` drains outstanding prefix windows and ensures D$ state transitions before capabilities are handed to other cores or devices.

### DMA visibility & coherence

* enid inbound writes that do not participate in coherence use the ACF `NC` path and land behind the D$ write-back buffers; software must invalidate affected lines before handing buffers to devices.
* Outbound DMA reads that see dirty cache lines rely on write-back completion; firmware toggles `DMACTL.sync_dma` to guarantee posted writes reach memory before ringing doorbells.
* Coherent accelerators must speak the ACF protocol and honour the capability metadata delivered with each `RespData`. Non-coherent clients receive capabilities marked non-cacheable so they never bypass the required fences.

---

## Pipeline & Timing

```
IA -> IF -> XT -> ID -> EX -> MA -> MF -> WB
```

* **Single-cycle** micro-ops from ID->WB on cache hits (documented with uppercase mnemonics in `details/micro-ops.md`).
* Misses stall MA/MF.
* XT enforces packing rules: **branch/indirect targets land on syllable 0** of a bundle.
* The micro-ops/ISA split enables `jump_sub/return/push/pop` without multi-cycle instructions; sequencing lives in `details/micro-ops.md`.

### Hazard considerations

- **Syllable alignment guard:** variable 12/24/48-bit syllables demand that every control-flow target land on bundle slot 0; a malformed encoder or bad patch will leave IF/XT misaligned until the pipe flushes.
- **Redirect latency:** branches resolve in XT and hardware traps drain the full IA->WB pipe, so each redirect inserts bubbles and mishandled `STATUS.IE` updates can reopen the window before state is saved.
- **Direct-mapped cache conflicts:** the 16x16-BAU I$ and D$ are single-way; competing addresses thrash a line and hold MA/MF stalled, back-pressuring earlier stages.
- **Packed lane read-modify-write:** memory only moves whole BAUs, forcing `cap_load/modify/cap_store`; pre-emption or another hart between the load and store can clobber the untouched lane.
- **Multi-BAU capability spills:** capabilities occupy two BAUs in memory and on the ABI stack, so faults between halves risk exposing a torn capability image.
- **Upper-immediate latch misuse:** any `.ext` consumer relies on a fresh `upper_imm`; failing to clear or refill the UI latch silently builds the wrong wide immediate without a validity trap.
- **Packed saturation visibility:** `.s` packed ops only report summary NZCV flags unless the optional lane mask CSR is implemented, hiding per-lane overflow from software that expects it.
- **TLB size-bit aliasing:** large-page entries bypass the final level; inconsistent size bits or stale ASIDs can hand different permission sets to the same BAU, generating capability or privilege faults at MA/MF.
- **enid DMA visibility:** failing to fence or flush around non-coherent enid transfers leaves software observing stale data or double-writing queue entries; drivers must toggle `DMACTL.sync_dma` around doorbells so posted writes settle before reuse.

---
