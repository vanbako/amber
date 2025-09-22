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
* **Syscall is *not* implemented via software interrupts.** A dedicated `sys_call` micro-op raises synchronous trap cause `Syscall` (without setting `CAUSE[47]`), keeping the ABI stable even if software interrupts are disabled.
* Handlers return with `sys_return`, re-enabling interrupts according to the saved `STATUS` image.

---

## Caches (16x16 BAU each)

* **I\$:** 16 lines x 16 BAU/line (**256 BAU** total). Direct-mapped MVP.
  Fetch delivers BAU-aligned windows to **XT**; `cPCC` bounds and X-perm checked.
* **D\$:** 16 lines x 16 BAU/line (**256 BAU** total). Direct-mapped MVP, write-through (write-back later).
  Capability and MMU perms enforced at **MF**.

### DMA visibility & coherence

* enid inbound writes land in memory behind the D$ write-through path; the fabric is **not** hardware-coherent. Drivers must invalidate cache lines covering DMA destinations before handing buffers to devices.
* Outbound DMA reads observe the most recent committed D$ state because write-through pushes data to memory immediately; for explicit dirty data, software toggles the `DMACTL.sync_dma` fence before ringing an enid doorbell.
* Firmware can optionally grant read-only snoop permissions to accelerators that advertise coherency; otherwise, capabilities for shared buffers must be marked non-cacheable.
* The `DMACTL.sync_dma` bit also lets software force completion of all posted enid writes before sampling device status or recycling descriptors.

---

## Pipeline & Timing

```
IA -> IF -> XT -> ID -> EX -> MA -> MF -> WB
```

* **Single-cycle** micro-ops from ID->WB on cache hits.
* Misses stall MA/MF.
* XT enforces packing rules: **branch/indirect targets land on syllable 0** of a bundle.
* micro-ops/ISA split enables `jump_sub/return/push/pop` without multi-cycle instructions.

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
