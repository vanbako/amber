# AD48 Macro Assembler (`ad48_asm.py`)

This Python tool translates human-readable AD48 assembly into 48-bit machine
code suitable for `$readmemh` or simulation preload.  The assembler lives in
`tools/ad48_asm.py` and is intentionally dependency-free so it can run on
Python 3.9+ out of the box.

## Usage

```bash
python3 tools/ad48_asm.py path/to/program.asm              # writes memh to stdout
python3 tools/ad48_asm.py demo.asm -o build/demo.hex       # Verilog memh
python3 tools/ad48_asm.py demo.asm --format hex            # contiguous hex words
python3 tools/ad48_asm.py demo.asm --format bin -o demo.bin
```

Output formats:

- `memh` (default): emits `@addr` addressing directives and 12-hex-digit words
  compatible with `$readmemh`.
- `hex`: contiguous list of words without address directives.  Use
  `--base-address` to shift the start address when tools expect a single block.
- `bin`: raw 48-bit words packed as 6-byte big-endian records.

## Toolchain Driver

For multi-source programs or repeatable ROM builds use the manifest-driven
toolchain wrapper (`tools/ad48_toolchain.py`):

```bash
python3 tools/ad48_toolchain.py tools/examples/demo.toml
python3 tools/ad48_toolchain.py app.toml -o build/app.memh --format memh
python3 tools/ad48_toolchain.py tools/examples/demo.toml --run-sim
```

The manifest is standard TOML (Python 3.11+ can read it directly; on older
interpreters install `tomli` or rely on the built-in simplified parser).  At a
minimum provide a `[program]` table and one or more `[[segment]]` entries:

```toml
[program]
name = "demo"
format = "memh"         # memh | hex | bin
output = "demo.memh"    # optional, defaults to stdout when omitted
listing = "demo.lst"    # optional segment summary
# imem_output = "demo.imem.memh"   # optional explicit memh artefact for simulation/backends

[[segment]]
label = "demo"          # friendly name for diagnostics/listing
source = "demo.asm"     # path relative to this manifest
origin = 0              # optional base offset before placing the segment

[[backend]]
type = "simulation"     # run the Icarus-based smoke test
max_cycles = 64         # optional cycle guard (0 disables)
# vcd = "wave/demo.vcd" # optional waveform capture
```

All segment images are assembled independently, shifted by `origin`, and merged.
Overlapping addresses raise an error.  For contiguous `hex` output you may set
`base_address` in `[program]` (or via `--base-address`) to force the start word;
missing addresses are padded with `fill` (default zero).  When `listing` is set,
the tool emits a concise per-segment report to help validate layout.  Adding a
`[[backend]]` entry with `type = "simulation"` instructs the toolchain to build
`tools/sim/run_core_tb.v` together with the core RTL, preload the generated
instruction memory (the `imem_output` file if specified, otherwise an automatic
`<name>.memh` artefact), and execute it with Icarus Verilog.  The run halts when
the core asserts `halt` or when `max_cycles` is exceeded.  CLI switches such as
`--run-sim`, `--sim-max-cycles`, and `--sim-vcd` can be used to enable or tweak
the simulation flow without editing the manifest; the backend mechanism is
structured so a future `type = "gowin"` target can slot in without touching the
assembler or manifest format.

## Assembly Syntax Highlights

- **Mnemonics:** lower-case keywords (`copy`, `add`, `subtract`, `load`,
  `store`, `branch`, `jump`, `halt`, `nop`, `system`).  Suffixes follow the
  mnemonic (e.g. `branch.eq`, `branch.gt.u`, `jump.ind`).
- **Registers:** `a0`-`a7` and `d0`-`d7`.  The assembler enforces the same bank
  restrictions as the RTL (e.g. loads always target `d` registers, branch
  compares are `A` vs `D`).
- **Immediates & Expressions:** accept decimal, hex (`0x`), binary (`0b`), and
  parenthesised arithmetic.  Symbols, `.equ` constants, `.` (current address),
  and `pc` are valid in expressions.
- **Labels:** any identifier followed by `:`.  Multiple labels on a line are
  allowed.
- **Directives:**
  - `.equ NAME, expr` - define a constant (can also use `NAME: .equ expr`).
  - `.org expr` - move the program counter (useful for placing data blocks).
  - `.word expr, ...` - emit literal 48-bit words.

## Macro System

Macros provide reusable code templates without external preprocessing:

```asm
.macro loadimm reg, value
    copy \reg, a0
    add  \reg, \reg, \value
.endmacro

loadimm d2, 42
```

Macro parameters are referenced with `\name`.  Use `\@` to splice a unique
suffix when generating symbols inside the macro body.  A trailing `...` in the
parameter list captures variadic arguments (e.g. `.macro foo x, rest...`).

## Branch Helpers

Conditional branches map directly onto the hardware comparator set:

| Mnemonic            | Behaviour                           | Notes                 |
|---------------------|-------------------------------------|-----------------------|
| `branch.eq`         | `A == D`                            |                       |
| `branch.ne`         | `A != D`                            |                       |
| `branch.lt[.u]`     | signed/unsigned `<`                 |                       |
| `branch.ge[.u]`     | signed/unsigned `>=`                |                       |
| `branch.always`     | unconditional                       | no register operands  |
| `branch.gt[.u]`     | synthesised as `lt` + `ne` pair     | emits 2 instructions  |
| `branch.le[.u]`     | synthesised as `lt` + `eq` pair     | emits 2 instructions  |

When a branch expands to multiple hardware instructions the assembler accounts
for the additional words during label resolution automatically.

## Example Program

`tools/examples/demo.asm` assembles to a tiny countdown loop that demonstrates
macros, `.equ`, and branching:

```asm
.equ LOOP_COUNT, 5

.macro loadimm dst, value
    copy \dst, a0
    add  \dst, \dst, \value
.endmacro

start:
    loadimm d1, LOOP_COUNT
    copy a1, a0
    add  a1, a1, 1

loop:
    subtract d1, d1, 1
    branch.ne loop, a0, d1

    halt
```

Running `python3 tools/ad48_asm.py tools/examples/demo.asm` yields:

```
@0
109000000000
209200000005
101000000000
101200000001
209207FFFFFF
5020FFFFFFFE
FF0000000000
```

which can be fed straight into the AD48 testbenches or ROM initialisation.

## Future Extensions

- Pseudo-ops for structure layout (e.g. `.align`, `.space`).
- Optional listing output with mixed source/hex.
- Integration with a higher-level toolchain (assembler -> linker -> ROM image).
