# Minimal Implementation Checklist (48 & 24 only)

* [ ] 16x48 GPRs (`r0..r15`), `r0=0`; `SP`, `LR`, `SSP`.
* [ ] 8x96 capability regs; implicit `cDDC` and `cPCC`.
* [ ] Only BAU-wide loads/stores (`load/store` + capability variants); no 8/16/24-bit transfers.
* [ ] 12/24/48 decode + `XT` unpack; branch target rule; `land`.
* [ ] I$ & D$: 16x16-BAU lines; direct-mapped; cap/MMU checks at `MF`.
* [ ] MMU: 3-level walk (12+12+12) with `2^12` BAU pages; `2^24` BAU large pages at level-1.
* [ ] Scalar 48-bit ALU + packed 24-bit ALU (`.u` & `.s`).
* [ ] CSR access; `upper_imm` + UI latch for wide immediates.
* [ ] Traps/IRQs; `jump_sub/return/push/pop` via micro-ops; HFCP token check.
* [ ] enid PHY + SerDes bring-up (lane training, framing, link reset).
* [ ] enid 3-port root complex with MSI bridge and capability validation.
* [ ] enid switch enumeration firmware: discover ports, allocate MMIO/DMA capabilities, program MSI vectors.
* [ ] Driver-model hooks for enid modules (GPU, audio, storage, HID, network, NPU) covering MMIO, DMA, and interrupt setup.

