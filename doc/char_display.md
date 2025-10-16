Per-cell layout is **4 × 24-bit = 96 bits**:

* **CHAR24** (only ASCII low 8 bits used for now)
* **FG24** (foreground RGB888)
* **BG24** (background RGB888)
* **MOD24** (flags/modifiers)

Everything else (48-bit 2-pixels/word pipe, alpha blend over your bg/ov layers, cursor on top) stays the same.

---

# Cell layout (96-bit)

```
[95:72] CHAR24   // ASCII in [7:0], [23:8] reserved
[71:48] FG24     // RGB888
[47:24] BG24     // RGB888
[23:0]  MOD24    // flags (below)
```

**MOD24 bits (updated):**

```
[0]   BG_EN        // 1 = draw solid BG using BG24 for 0-bits; 0 = 0-bits are transparent
[1]   INV          // invert FG/BG roles on glyph bits
[2]   UL           // underline (last 2 rows)
[3]   BOLD         // 1px pseudo-bold
[4]   BLINK        // blink via global counter
[5]   DIM          // halve FG intensity before 6-bit quantize
[6]   OPAQUE_FG    // 1 = force FG α=63 (default is 63 anyway; reserve for future)
[7]   T2A_BG       // 1 = make BG transparent (α=0) even when BG_EN=1
[15:8] RESERVED
[23:16] RESERVED   // keep headroom; you can later add per-cell alphas, doublewidth, etc.
```

---

# Memory footprint

* Grid: **160×45 = 7,200 cells** (8×16 font @ 1280×720)
* **96 bits/cell = 12 bytes** → **86,400 bytes ≈ 84.4 KiB**
* Font ROM unchanged: **4 KiB**
* Still comfortably in on-chip SRAM.

---

# AD48 integration (single FPGA MMIO)

The AD48 core issues full-word (48-bit) loads and stores with no byte enables and treats addresses as word indices. Place the text block on the on-chip peripheral fabric at a convenient base word (`TEXT_MMIO_BASE`). Firmware performs plain `ST` instructions to the word offsets below; the hardware ignores the upper 16 bits of each write so software can keep using ordinary 48-bit values. While the compositor and CPU share one FPGA, this is just another trusted MMIO window that can be covered by a supervisor capability.

## Register aperture (word offsets)

Offsets are given relative to `TEXT_MMIO_BASE` and keep the earlier map layout (old byte offsets 0x58..0x68 translate to word offsets 0x16..0x19 once you divide by four). Reserve four contiguous AD48 words for the 96-bit write aperture:

| Word offset | Name | Bits used | Description |
| ----------- | ---- | --------- | ----------- |
| `+0x16` | `TEXT_WADDR` | `[12:0]` | Linear cell index (`row * COLS + col`). Upper bits read as zero. |
| `+0x17` | `TEXT_WDATA_LO` | `[23:0]=CHAR24`, `[47:24]=FG24` | Lower half of the cell, little-endian inside the 48-bit word. |
| `+0x18` | `TEXT_WDATA_HI` | `[23:0]=BG24`, `[47:24]=MOD24` | Upper half of the cell. |
| `+0x19` | `TEXT_WCOMMIT` | bit `0` | Write `1` to latch `{WDATA_HI, WDATA_LO}` into `TEXT_WADDR`. Auto-clears on accept. |

Remove `GLOBAL_BG_RGB` unless firmware still needs a fallback fill colour. The rest of the legacy register map (`TEXT_CTRL`, `TEXT_ORIGIN`, `TEXT_SIZE`, `BLINK_DIV`, …) stays untouched and continues to use one AD48 word each.

Cell packing is simple: the lower 24 bits of each data word carry the structure listed earlier, the upper 24 bits carry the companion colour/modifier field, and the design currently expects software to zero the reserved CHAR bits `[23:8]`.

---

# Blend rules (unchanged at the interface)

* The text layer still emits **R6G6B6A6** pixels (packed 2×24→48-bit) and blends on top of bg+ov using your compositor’s alpha math.
* **FG pixels:** α = 63 (unless you later add per-cell alpha).
* **BG pixels:** if `BG_EN=1` then drawn using BG24; if `T2A_BG=1` set α=0, else α=63. If `BG_EN=0`, zero-bits are fully transparent.

---

# Verilog updates (key deltas)

### 1) Cell RAM: 96-bit wide

You can infer as 3×32-bit RAMs ganged together, or a vendor 96-bit primitive if available.

```verilog
module cellram96 #(
  parameter COLS=160, ROWS=45
) (
  // CPU write port
  input               clk_cpu,
  input               we_cpu,
  input  [12:0]       waddr,    // 0..7199
  input  [95:0]       wdata,

  // Renderer read port
  input               clk_pix,
  input  [12:0]       raddr,
  output reg [95:0]   rdata
);
// Implement as true dual-port RAM (3x32 or 4x24 banks are fine)
endmodule
```

### 2) Renderer: unpack 96-bit and use per-cell BG

Only the changed pieces are shown (relative to the earlier `text_scanline` skeleton).

```verilog
// Unpack 96-bit cell
wire [23:0] CHAR = cell_rdata[95:72];
wire [23:0] FG24 = cell_rdata[71:48];
wire [23:0] BG24 = cell_rdata[47:24];
wire [23:0] MOD  = cell_rdata[23:0];

// Modifiers
wire bg_en   = MOD[0];
wire inv     = MOD[1];
wire ul      = MOD[2];
wire bold    = MOD[3];
wire blinkf  = MOD[4];
wire dim     = MOD[5];
wire opaque_fg = MOD[6];     // reserved for future; currently FG α=63
wire t2a_bg  = MOD[7];

// Select glyph bit (with BOLD option) just like before
// ... (same bold/underline/blink logic as earlier) ...

// FG/BG choice with INV
wire ink_bit = /* bold & underline & blink pipeline from prior design */;
wire use_fg  = ink_bit ^ inv;
wire use_bg  = ~use_fg && bg_en;

// 888 -> 6-bit helpers
function [5:0] c8_to_6(input [7:0] c8); c8_to_6 = c8[7:2]; endfunction

// Foreground 6-bit (DIM halves FG brightness)
wire [5:0] r6_fg = dim ? (c8_to_6(FG24[23:16]) >> 1) : c8_to_6(FG24[23:16]);
wire [5:0] g6_fg = dim ? (c8_to_6(FG24[15: 8]) >> 1) : c8_to_6(FG24[15: 8]);
wire [5:0] b6_fg = dim ? (c8_to_6(FG24[ 7: 0]) >> 1) : c8_to_6(FG24[ 7: 0]);

// Background 6-bit (no DIM by default)
wire [5:0] r6_bg = c8_to_6(BG24[23:16]);
wire [5:0] g6_bg = c8_to_6(BG24[15: 8]);
wire [5:0] b6_bg = c8_to_6(BG24[ 7: 0]);

// Compose one pixel (R6 G6 B6 A6)
reg [23:0] px24;
always @(*) begin
  if (use_fg)           px24 = {r6_fg, g6_fg, b6_fg, 6'd63};                  // FG opaque
  else if (use_bg)      px24 = {r6_bg, g6_bg, b6_bg, (t2a_bg ? 6'd0 : 6'd63)}; // BG as set
  else                  px24 = {6'd0,  6'd0,  6'd0,  6'd0};                    // transparent
end

// Pack 2×24 → 48 and stream out (unchanged)
```

### 3) MMIO write path: 2×48 commits per cell

Pseudocode for the CPU side:

```c
// write one cell at (row,col)
uint64_t cell[2];
uint64_t idx = (uint64_t)(row * COLS + col);

text_pack_cell(ch, fg_rgb888, bg_rgb888, mods, cell);

text_mmio[TEXT_WADDR]     = idx & 0x0000FFFFFFFFFFFFULL;  // AD48 stores are 48-bit words
text_mmio[TEXT_WDATA_LO]  = cell[0];
text_mmio[TEXT_WDATA_HI]  = cell[1];
text_mmio[TEXT_WCOMMIT]   = 1;   // any non-zero value toggles the commit strobe
```

A cell pack helper (host-side):

```c
// CHAR in low 8 bits for now, FG/BG are 0xRRGGBB, MOD is the 24-bit bitfield above.
// Returns two 48-bit words (stored in 64-bit containers).
static inline void text_pack_cell(uint8_t ch,
                                  uint32_t fg_rgb888,
                                  uint32_t bg_rgb888,
                                  uint32_t mods,
                                  uint64_t out[2]) {
  uint64_t char24 = (uint64_t)(ch & 0xFF);           // higher CHAR bits stay zero
  uint64_t fg24   = (uint64_t)(fg_rgb888 & 0xFFFFFF);
  uint64_t bg24   = (uint64_t)(bg_rgb888 & 0xFFFFFF);
  uint64_t mod24  = (uint64_t)(mods & 0xFFFFFF);

  out[0] = (fg24 << 24)  | char24;
  out[1] = (mod24 << 24) | bg24;

  out[0] &= 0x0000FFFFFFFFFFFFULL;
  out[1] &= 0x0000FFFFFFFFFFFFULL;
}
```

> AD48 stores enumerate words, not bytes. Keeping the packing as `{FG24, CHAR24}` and `{MOD24, BG24}` matches the Verilog slice notation and keeps firmware-side shifts simple.

---

# Integration recap

* **Positioning & size**: same `TEXT_ORIGIN` and `TEXT_SIZE` (cols, rows); defaults still 160×45 @ 8×16 font.
* **Pipeline**: text overlay renders per scanline in the pixel domain → emits 48-bit words → alpha-blend **over** (bg ⊕ overlay) and **under** the hardware cursor.
* **Timing**: unchanged; the work per pixel is still tiny (glyph bit + mux + two 888→6 paths), so the 74.25 MHz domain remains comfortable with 1–2 pipeline stages.

---

# Dual-FPGA / PCIe-style roadmap

When the compositor and text renderer migrate to their own FPGA, keep the MMIO contract intact and bridge it over the PCIe-like link:

- **Posted command queue**: capture each AD48 store to `TEXT_WADDR/WDATA/WCOMMIT` and packetise it as `{offset, data}` entries in a small FIFO that streams across the link. Preserving write ordering guarantees the cell RAM sees the same sequence it would on-chip.
- **Doorbell & flow control**: reuse the `TEXT_WCOMMIT` write as the doorbell. The bridge can expose a credits register so firmware can throttle bulk updates (e.g. during full-screen paints).
- **Events back to AD48**: surface `blink_toggle`, underflow, or error conditions as interrupts delivered through the existing capability-level interrupt path so the CPU firmware does not care whether the compositor is local or remote.

The remote FPGA recreates the same register file and cell RAM update logic on its side, latching the 96-bit payload into BRAM once both halves arrive. That way firmware, toolchains, and capability assignments stay unchanged between the single-FPGA and dual-FPGA deployments.
