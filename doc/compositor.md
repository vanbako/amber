Absolutely—let’s nail down a minimal-yet-solid compositor you can build in Verilog on the GW5AST (Tang Mega 138K + DVI dock), targeting **1280×720 @ 60 Hz**. I’ll make the memory format **48-bit** so each 48-bit granule contains **two 24-bit pixels** (RGBA with **6 bits per channel**: R6 G6 B6 A6). This plays perfectly with Gowin’s 48-bit BRAM/SRAM.

---

# MVP goals (what it should do)

1. **Scanout** a framebuffer to the DVI port at 720p60.
2. **Compositing:** 1 background layer + 1 overlay layer (per-pixel 6-bit alpha), plus an optional **hardware cursor**.
3. **Tear-free swap** via double buffering (per-layer).
4. **Simple register block** to point to base addresses, strides, and enable bits.
5. **Line-buffered pipeline** using 48-bit BRAM granules end-to-end.

This is already “modern enough” (alpha blend + cursor) while staying small.

---

# Key numbers (so we size/budget correctly)

* **CEA-720p60 pixel clock:** 74.25 MHz.
* **Active pixels/s:** 1280×720×60 = **55.296 MPix/s**.
* **Per-pixel format:** RGBA **R6G6B6A6** → **24 bits**.
* **Scanout read bandwidth (1 layer):** 55.296 M × 3 B ≈ **165.9 MB/s**.
* **Two layers (bg + overlay):** ≈ **331.8 MB/s** (+ overhead still far below DDR3 limits).
* **Frame size (1 layer, 720p):** 1280×720×3 B = **2,764,800 bytes ≈ 2.64 MiB**.
* **Words/line in 48-bit packing:** 1280 / 2 = **640** (each word = 2 pixels).
* **Line size:** 640 × 6 B = **3,840 B**.
* **BRAM for line buffers:** tiny (see below), fits easily into your S-/B-SRAM.

---

# Video timing (fixed 720p60)

Use CEA-861 timing (you can hardcode constants):

* **Horizontal:** total 1650, active 1280, front porch 110, sync 40, back porch 220.
* **Vertical:** total 750, active 720, front porch 5, sync 5, back porch 20.

You’ll generate hcnt/vcnt, hsync/vsync, de (data enable), and a **pixel enable** at 74.25 MHz.

---

# Output: DVI (TMDS)

* 3 TMDS data lanes + 1 TMDS clock.
* TMDS rate is **10× pixel clock** ≈ **742.5 Mb/s** per lane.
* Use the common TMDS 8b→10b encoder per color; clock lane runs at pixel clock.
* Expand 6-bit channels to 8-bit for TMDS by MSB replication:
  `c8 = {c6, c6[5:4]}` (or `{c6, c6[5:4]}` ≈ ×4 + copy top 2 bits).
* If you want nicer gradients: insert an optional 6→8 **gamma LUT** (small 64-entry ROM per channel).

> Practical: keep your whole pixel pipe synchronous to the 74.25 MHz pixel clock. The high-speed TMDS serializers use vendor SERDES or DDR output primitives in a 5× or 10× domain as required by the Gowin DVI reference designs.

---

# Memory format (48-bit granule)

Each 48-bit word holds **two pixels**, each pixel = 24 bits in **R6 G6 B6 A6** order.

* **Packing inside a 48-bit word:**

  * Bits **[47:24] = Pixel0 {R[47:42], G[41:36], B[35:30], A[29:24]}**
  * Bits **[23:0]  = Pixel1 {R[23:18], G[17:12], B[11:6],  A[5:0]}**
* **Stride (words per line):** `words_per_line = ceil(width / 2)`; for 1280 it’s **640**.
* **Addressing:** linear, row-major; next line = base + `words_per_line`.
* **Endianness:** pick one (and stick to it). I recommend **MS pixel first** as above—it lines up with natural [47:24]/[23:0] slices in Verilog.

You can still store in DDR3 as **64-bit bursts** for efficiency; the **scanout DMA** will **unpack 64→48** on the fly into the 48-bit line buffers (details below).

---

# Block diagram (MVP)

```
                 +----------------+
 DDR3 64-bit --> |  Read DMA (bg) | --\
                 +----------------+    \
                                          +------------------+    +------------------+    +----------------------+
                 +----------------+    --| Line Buffers (bg) |--> | Line Buffers (ov) |--> |  Cursor Blender     |
 DDR3 64-bit --> |  Read DMA (ov) | --/  +------------------+    +------------------+    +----------------------+
                 +----------------+
                                                                                                     |
                                                                                          +----------------------+
                                                                                          |   Alpha Blender      |
                                                                                          | (R6G6B6 × A6)        |
                                                                                          +----------+-----------+
                                                                                                     |
                                                                                       +---------------------------+
                                                                                       |  6->8 expand / gamma LUT  |
                                                                                       +------------+--------------+
                                                                                                    |
                                                                                       +---------------------------+
                                                                                       |   TMDS Enc + Serializer   |
                                                                                       +---------------------------+
                                                                                                    |
                                                                                                DVI connector
```

**Why dual line buffers per layer?**
To decouple DDR bursts from pixel timing. For each layer, use a **ping-pong line buffer** (A/B). While A is being scanned out, DMA fills B, then swap.

* **Per-line BRAM use (per layer):** 3,840 B ≈ **3.75 KiB**.
  Ping-pong = **~7.5 KiB**/layer. Two layers → **~15 KiB**.
  Cursor adds a tiny FIFO/ROM (see below). You’ve got **~900 KiB** SRAM total—plenty.

---

# DMA (read) details

* Drive DDR3 with long **burst reads**, line by line.
* **Background layer:** fixed full-screen; base, stride, height in registers.
* **Overlay layer:** can be the same size or smaller; add **x/y** top-left registers.
* **Cursor:** 64×64 AR6G6B6 (12,288 B total) pre-loaded in BRAM or small SRAM, fetched by a small **per-line cursor fetcher** (not from DDR).

**64-→48 bridge (optional but practical):**

* DDR returns 64-bit beats. Accumulate **3×64 = 192 bits**, which packs cleanly into **4×48-bit** words (192/48=4).
* Simple packer FSM: every 3 DDR beats → push 4 words into a 48-bit-wide FIFO feeding the line buffer.

This keeps your internal bus **48-bit** all the way to the blender, matching the RAMs.

---

# Compositing math (R6G6B6 × A6)

Assume overlay has per-pixel alpha A6 (0..63). Background alpha can be ignored or treated as opaque.

For each color channel (6-bit):

```
out = (fg * A + bg * (63 - A) + 31) >> 6   // rounded
```

* `fg`, `bg`, `A` are 0..63.
* Do per-channel; clip to 0..63.
* Alpha is **straight** (not premultiplied). If you prefer premultiplied in memory, drop the second term and skip the extra multiply.

**Cursor** (ARGB6): do the same blend but only in the cursor rectangle (x ≤ hcnt < x+64, y ≤ vcnt < y+64). Cursor sits **on top** of overlay.

---

# AD48 integration (single FPGA)

While the compositor lives on the same FPGA as the AD48 CPU, expose it as a 48-bit MMIO slave on the core’s peripheral fabric:

- AD48 load/store operations always transfer a full 48-bit word and use word indices. Assign a base word (`GFX_MMIO_BASE`) and ignore the upper 16 bits on narrow control fields.
- Treat surface bases and strides as counts of packed 48-bit pixels. The DMA multiplies by 6 bytes internally when driving the external DDR controller, so firmware never handles byte addresses directly.
- Protect the window with a supervisor capability so only the compositor driver can poke it; unprivileged tasks interact through whatever syscall/API you provide.

---

# Register map (AD48 word offsets)

Offsets below are relative to `GFX_MMIO_BASE` **in AD48 words** (divide the historical byte offsets by four). Each entry occupies one 48-bit word; unused bits read as zero and should be written as zero.

| Word offset | Name | Bits used | Notes |
| ----------- | ---- | --------- | ----- |
| `+0x00` | `GFX_CTRL` | `[0]=enable`, `[1]=overlay_enable`, `[2]=cursor_enable`, `[3]=vblank_swap` | Set `vblank_swap` when you want base/stride/size updates to take effect at the next blanking interval. |
| `+0x01` | `GFX_IRQ_STATUS` | `[0]=vblank`, `[1]=dma_underflow_bg`, `[2]=dma_underflow_ov` | Write-one-to-clear. Route into the AD48 IRQ fabric. |
| `+0x02` | `BG_BASE` | `[47:0]` | Word index of the background surface inside the shared DDR aperture. Hardware multiplies by 6 to form byte addresses. |
| `+0x03` | `BG_STRIDE` | `[23:0]` | Number of packed 48-bit words per line. Defaults to 640 for 1280-wide. |
| `+0x04` | `OV_BASE` | `[47:0]` | Overlay surface word index. |
| `+0x05` | `OV_STRIDE` | `[23:0]` | Overlay words per line. |
| `+0x06` | `OV_X` | `[11:0]` | Overlay X origin in pixels (0..1279). |
| `+0x07` | `OV_Y` | `[11:0]` | Overlay Y origin in lines (0..719). |
| `+0x08` | `CUR_X` | `[11:0]` | Cursor X origin. |
| `+0x09` | `CUR_Y` | `[11:0]` | Cursor Y origin. |
| `+0x0A` | `GFX_SWAP` | bit `0` | Pulse `1` to copy shadow base/stride/size registers into the active set when `CTRL.vblank_swap` is set. |
| `+0x0B` | `BG_SIZE` | `[23:0]=width_pix`, `[47:24]=height_lines` | Program once (1280×720) unless you add dynamic sizing. |
| `+0x0C` | `OV_SIZE` | `[23:0]=width_pix`, `[47:24]=height_lines` | Overlay dimensions. |

Keep it simple at first: hardcode 1280×720 in RTL, then add the *_SIZE regs later.

---

# Step-by-step build plan (recommended order)

1. **Video timing generator** (720p constants) → drive de/hsync/vsync.
2. **Color bars test** → TMDS encoder + serializer → verify 720p60 on a monitor.
3. **Single-layer scanout**

   * Implement 64→48 packing FIFO and 48-bit line buffer ping-pong.
   * DDR read DMA line walker (base/stride).
   * Show a test pattern uploaded to DDR.
4. **Two-layer compositing**

   * Second DMA + line buffer.
   * Alpha blender (6×6→12-bit multiplies + adder; one or two pipeline stages).
5. **Cursor**

   * Small BRAM with 64×64 AR6G6B6.
   * Gate/blend only in the cursor window.
6. **VBlank-safe double buffering**

   * Shadow regs + SWAP at VBlank.
7. **Quality nits (optional)**

   * 6→8 gamma LUTs, 2D dither when truncating, IRQ/underflow counters, simple color-key in overlay path (if you want a “chrome” window style).

---

# Verilog-level interface sketches

**48-bit pixel packing helpers**

```verilog
// Pixel layout inside 24 bits: {R[23:18], G[17:12], B[11:6], A[5:0]}

// Pack two 24-bit pixels into one 48-bit word
function [47:0] pack2x24(input [23:0] p0, input [23:0] p1);
  pack2x24 = {p0, p1};
endfunction

// Unpack
function [23:0] unpack_hi(input [47:0] w);
  unpack_hi = w[47:24];
endfunction
function [23:0] unpack_lo(input [47:0] w);
  unpack_lo = w[23:0];
endfunction

// Split 24-bit pixel into channels
function [5:0] chR(input [23:0] p); chR = p[23:18]; endfunction
function [5:0] chG(input [23:0] p); chG = p[17:12]; endfunction
function [5:0] chB(input [23:0] p); chB = p[11:6];  endfunction
function [5:0] chA(input [23:0] p); chA = p[5:0];   endfunction

// Recombine channels into 24-bit pixel
function [23:0] make_px(input [5:0] r, input [5:0] g, input [5:0] b, input [5:0] a);
  make_px = {r, g, b, a};
endfunction
```

**Alpha blend (one channel, rounded)**

```verilog
// 6-bit channel, 6-bit alpha
function [5:0] blend6(input [5:0] fg, input [5:0] bg, input [5:0] a);
  // 0..63 × 0..63 => 12-bit
  // out = (fg*a + bg*(63-a) + 31) >> 6
  reg [11:0] m1, m2, sum;
  begin
    m1  = fg * a;
    m2  = bg * (6'd63 - a);
    sum = m1 + m2 + 12'd31;
    blend6 = sum[11:6]; // >> 6 with rounding
  end
endfunction
```

**6→8 expansion (for TMDS input)**

```verilog
function [7:0] c6_to_c8(input [5:0] c6);
  // replicate MSBs: xxxx xx -> xxxx xx xx
  c6_to_c8 = {c6, c6[5:4]};
endfunction
```

**Line buffer RAM (48-bit wide, 640 deep)**

Use vendor 48-bit dual-port BRAMs. Depth per line: **640** words. Make two instances (ping-pong) per layer.

```verilog
// Pseudo-port: write side (DMA clock), read side (pix clock)
module linebuf48 #(
  parameter DEPTH = 640
)(
  input              clk_wr, we,
  input  [$clog2(DEPTH)-1:0] waddr,
  input  [47:0]      wdata,
  input              clk_rd,
  input  [$clog2(DEPTH)-1:0] raddr,
  output [47:0]      rdata
);
// infer/vendor-primitive 48-bit true dual-port RAM
endmodule
```

**DMA line walker (concept)**

* For line `y`: DDR address = `base + y*stride`.
* Read `words_per_line` *48-bit* equivalents via **3×64→4×48** packer into the line buffer.
* Signal **line_ready** before the display enters that line’s active `de`.

---

# Clocking & domains

* **pix_clk = 74.25 MHz**: timing gen, blender, LUTs, 48-bit line-buffer reads.
* **mem_clk**: DDR user clock from the controller (e.g., 100–200 MHz typical).
* **tmds_5x / 10x**: serializer clock(s).
* Async FIFOs at **DMA→linebuffer** boundary (or TDP BRAM with two clocks) will keep domains clean.

---

# Roadmap to a dedicated display FPGA

When the compositor migrates onto its own FPGA behind a PCIe-like link, keep the software contract identical and bridge MMIO traffic plus bulk updates:

- **Posted register writes**: capture each AD48 `ST` into an ordered command stream `{offset, data}` and send it across the link. The remote FPGA replays the writes into its local register file so firmware does not notice the split.
- **Surface ownership**: either expose a shared memory window that both sides can DMA, or provide a lightweight upload queue (e.g., line-copy descriptors) that the remote compositor consumes to populate its local DDR. In both cases, keep base/stride units in 48-bit words so drivers stay unchanged.
- **Events back to AD48**: forward vblank pulses, underflow flags, and link errors as interrupts by writing `GFX_IRQ_STATUS` over the return path or via a dedicated message queue.
- **Text overlay alignment**: the text MMIO updates described in `char_display.md` can share the same transport—treat them as high-rate posted writes that land in the remote cell RAM before scanout reaches the affected lines.

A thin bridge (command FIFO + credit counter + completion channel) preserves ordering, provides flow control, and lets you keep the exact same supervisor capability layout. Only the physical transport changes when you jump from single-FPGA to the dual-FPGA/PCIe-style deployment.

---

# What to implement first (tiny slices of work)

1. Timing + DVI **color bars** (no memory).
2. Single layer: DMA → 48-bit line buffer → TMDS.
3. Add overlay path, alpha blender.
4. Add cursor and swap-at-vblank.

I kept everything faithful to your **48-bit granule** idea so two pixels live in one word all the way through the SRAMs and blend unit. When you’re ready, I can help you stub the first modules (timing core, TMDS encoder shell, and a simple line-reader DMA) and we’ll get pixels on screen fast.
