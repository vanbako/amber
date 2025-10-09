# AD48 Toolchain (`ad48_toolchain.py`)

The manifest-driven toolchain orchestrates assembling one or more AD48
program segments and prepares ROM artefacts for simulation or integration.
It wraps the standalone assembler (`tools/ad48_asm.py`), so only Python 3.9+
is required. On Python 3.11+ the bundled `tomllib` parser is used; older
interpreters can optionally install `tomli`, otherwise the lightweight
fallback parser embedded in the script is used automatically.

## Quick Start

```bash
python3 tools/ad48_toolchain.py tools/examples/demo.toml
python3 tools/ad48_toolchain.py tools/examples/demo.toml --run-sim
python3 tools/ad48_toolchain.py tools/examples/demo.toml --format hex -o build/demo.hex
```

- Source paths inside the manifest are resolved relative to the manifest file.
- Outputs default to the manifest directory unless overridden.
- When `--run-sim` (or a `[[backend]]` simulation entry) is present the tool
  emits a `$readmemh` image and launches the bundled Icarus Verilog smoke test.

## Manifest Layout

Manifests are standard TOML. The example at `tools/examples/demo.toml`
illustrates each section.

### `[program]`

| Key            | Type     | Description |
| -------------- | -------- | ----------- |
| `name`         | string   | Friendly name used for artefact filenames. Defaults to the manifest stem. |
| `format`       | string   | Output format: `memh`, `hex`, or `bin`. Defaults to `memh`. |
| `output`       | string   | Optional path for the primary artefact. When omitted the image is written to stdout. |
| `listing`      | string   | Optional Markdown summary generated via `_write_listing`. |
| `imem_output`  | string   | Explicit `$readmemh` file used by simulation backends. Defaults to `output` when `format = "memh"`. |
| `base_address` | integer  | Starting address for contiguous `hex` exports. Defaults to the minimum populated address. |
| `fill`         | integer  | Filler word (0 <= value < 2^48) for gaps in contiguous `hex` output. Defaults to 0. |

### `[[segment]]`

Each segment assembles an individual source file, optionally shifted in memory.

| Key       | Type     | Description |
| --------- | -------- | ----------- |
| `source`  | string   | Required path to an `.asm` file. |
| `origin`  | integer  | Starting address offset applied to the assembled words. Defaults to 0. |
| `label`   | string   | Friendly name used in diagnostics and listings. Defaults to the source stem. |

Segments are assembled independently and merged. Address collisions are
reported with the overlapping segment names.

### `[[backend]]`

Backends extend the assembly flow. The only implemented backend today is
`type = "simulation"`, which drives the Icarus Verilog test bench.

| Key           | Type            | Description |
| ------------- | --------------- | ----------- |
| `top`         | string          | Simulation top module. Defaults to `run_core_tb`. |
| `max_cycles`  | integer         | Optional runtime guard; `0` disables the timeout. |
| `vcd`         | string          | When set, writes a waveform via `+VCD=...`. |
| `sources`     | string / list   | Extra Verilog sources appended to the default CPU RTL set. |
| `include_dirs`| string / list   | Additional `-I` include directories. |
| `defines`     | table / list    | Preprocessor defines (value-less entries default to `"1"`). |
| `parameters`  | table           | Verilog parameters applied with `-P <top>.<name>=<value>`. |

Use `enable = false` to keep an entry in the manifest without running it.
Non-simulation backends are parsed but currently only log a "not implemented"
message.

## CLI Overrides

All manifest fields can be overridden from the command line:

- `-o/--output`, `--format`, `--base-address`, and `--fill` replace the
  corresponding `[program]` values.
- `--imem-output` forces a concrete `$readmemh` file, useful for running
  simulations without modifying the manifest.
- `--no-listing` suppresses listing generation even when `listing` is set.
- `--run-sim` runs the default simulation backend without a manifest entry.
- `--sim-max-cycles` and `--sim-vcd` adjust simulation limits across all
  declared simulation backends.

## Simulation Workflow

The simulation backend compiles `tools/sim/run_core_tb.v` together with the CPU
RTL under `src/rtl/` using `iverilog -g2012`. The assembled instruction memory
image is passed to the test bench via a `+IMEM=<path>` plusarg while optional
`+MAX_CYCLES` and `+VCD` arguments enforce runtime limits and waveform logging.
Any additional sources, include directories, parameters, or defines declared in
the manifest are appended to the compile and run commands.

The build artefacts for each backend live under
`<manifest>/build/sim/<backend-name>/`. The compiled VVP executable is reused on
subsequent runs until the inputs change.

## Listings

When `listing` is set (and `--no-listing` is not supplied) the tool produces a
Markdown summary of each segment showing the populated address range after
applying the declared `origin`. Listings are handy for reviewing ROM layout or
verifying that multiple segments do not collide.

## Exit Codes

The tool returns a non-zero status on manifest parsing errors, assembly
failures, overlapping segments, or subprocess issues (missing commands, failed
simulation runs, etc.). Check the console output for diagnostics such as
"command not found" or "address collision" when troubleshooting.
