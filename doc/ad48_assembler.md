# AD48 Macro Assembler (`ad48_asm.py`)

This Python tool translates human-readable AD48 assembly into 48-bit machine
code suitable for `$readmemh` or simulation preload.  The assembler lives in
`tools/ad48_asm.py` and is intentionally dependency-free so it can run on
Python 3.9+ out of the box.

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

## Assembly Syntax Highlights

- **Mnemonics:** lower-case keywords (`copy`, `add`, `subtract`, `load`,
  `store`, `branch`, `jump`, `halt`, `nop`, `system`).  Suffixes follow the
  mnemonic (e.g. `branch.eq`, `branch.gt.u`, `jump.ind`).
- **Registers:** `a0`–`a7` and `d0`–`d7`.  The assembler enforces the same bank
  restrictions as the RTL (e.g. loads always target `d` registers, branch
  compares are `A` vs `D`).
- **Immediates & Expressions:** accept decimal, hex (`0x`), binary (`0b`), and
  parenthesised arithmetic.  Symbols, `.equ` constants, `.` (current address),
  and `pc` are valid in expressions.
- **Labels:** any identifier followed by `:`.  Multiple labels on a line are
  allowed.
- **Directives:**
  - `.equ NAME, expr` — define a constant (can also use `NAME: .equ expr`).
  - `.org expr` — move the program counter (useful for placing data blocks).
  - `.word expr, ...` — emit literal 48-bit words.

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
- Integration with a higher-level toolchain (assembler → linker → ROM image).
