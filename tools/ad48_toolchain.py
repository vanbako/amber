#!/usr/bin/env python3
"""AD48 assembler toolchain driver.

This utility stitches together one or more assembly sources described in a
manifest file and emits a ROM image that can be consumed by the AD48 RTL or
testbenches.  It reuses the standalone assembler (`ad48_asm.py`) under the hood
so there are no external dependencies beyond the Python standard library.
"""

from __future__ import annotations

import argparse
import ast
import dataclasses
import pathlib
import shlex
import subprocess
import sys
from typing import Dict, List, Optional, Tuple

try:
    import tomllib  # Python 3.11+
except ModuleNotFoundError:  # pragma: no cover - Python <3.11 fallback
    try:
        import tomli as tomllib  # type: ignore
    except ModuleNotFoundError:
        tomllib = None  # type: ignore[assignment]

try:
    # Running as a script: tools directory is added to sys.path automatically.
    import ad48_asm
except ModuleNotFoundError:
    # Invoked as a module: add this file's parent directory to sys.path.
    sys.path.insert(0, str(pathlib.Path(__file__).resolve().parent))
    import ad48_asm

REPO_ROOT = pathlib.Path(__file__).resolve().parent.parent
SIM_TESTBENCH_DIR = REPO_ROOT / "tools" / "sim"
DEFAULT_SIM_TESTBENCH = SIM_TESTBENCH_DIR / "run_core_tb.v"
DEFAULT_SIM_RTL = [
    REPO_ROOT / "src" / "rtl" / "cpu_ad48.v",
    REPO_ROOT / "src" / "rtl" / "mem_access_unit.v",
    REPO_ROOT / "src" / "rtl" / "alu.v",
    REPO_ROOT / "src" / "rtl" / "regfiles.v",
    REPO_ROOT / "src" / "rtl" / "mem48.v",
    REPO_ROOT / "src" / "rtl" / "simple_soc_stub.v",
]
DEFAULT_SIM_INCLUDE_DIRS = [REPO_ROOT / "src" / "rtl"]


class ToolchainError(RuntimeError):
    """Raised when the toolchain manifest or assembly combining fails."""


def _strip_toml_comment(line: str) -> str:
    """Remove unquoted '#' comments from a TOML-ish line."""
    in_string = False
    quote_char = ""
    escaped = False
    for idx, ch in enumerate(line):
        if escaped:
            escaped = False
            continue
        if ch == "\\" and in_string:
            escaped = True
            continue
        if ch in ("'", '"'):
            if in_string and ch == quote_char:
                in_string = False
                quote_char = ""
            elif not in_string:
                in_string = True
                quote_char = ch
            continue
        if ch == "#" and not in_string:
            return line[:idx].strip()
    return line.strip()


def _parse_simple_value(value: str, path: pathlib.Path, lineno: int):
    try:
        return ast.literal_eval(value)
    except Exception as exc:  # pylint: disable=broad-except
        raise ToolchainError(f"{path}:{lineno}: unsupported value '{value}'") from exc


def _parse_manifest_text(text: str, path: pathlib.Path) -> Dict[str, object]:
    program: Dict[str, object] = {}
    program_seen = False
    segments: List[Dict[str, object]] = []
    backends: List[Dict[str, object]] = []
    current: Optional[Dict[str, object]] = None

    for lineno, raw in enumerate(text.splitlines(), start=1):
        line = _strip_toml_comment(raw)
        if not line:
            continue
        if line.startswith("[[") and line.endswith("]]"):
            name = line[2:-2].strip()
            if name in {"segment", "segments"}:
                current = {}
                segments.append(current)
                continue
            if name in {"backend", "backends"}:
                current = {}
                backends.append(current)
                continue
            raise ToolchainError(f"{path}:{lineno}: unsupported table '{name}'")
        if line.startswith("[") and line.endswith("]"):
            name = line[1:-1].strip()
            if name != "program":
                raise ToolchainError(f"{path}:{lineno}: unsupported section '[{name}]'")
            if program_seen:
                raise ToolchainError(f"{path}:{lineno}: duplicate [program] section")
            program_seen = True
            current = program
            continue
        if "=" not in line:
            raise ToolchainError(f"{path}:{lineno}: expected key=value pair")
        if current is None:
            raise ToolchainError(f"{path}:{lineno}: value defined outside a table")
        key, raw_value = map(str.strip, line.split("=", 1))
        current[key] = _parse_simple_value(raw_value, path, lineno)

    return {"program": program, "segment": segments, "backend": backends}


@dataclasses.dataclass
class SegmentSpec:
    path: pathlib.Path
    origin: int = 0
    label: str = ""


@dataclasses.dataclass
class BackendSpec:
    kind: str


@dataclasses.dataclass
class SimulationSpec(BackendSpec):
    name: str
    top: str = "run_core_tb"
    max_cycles: int = 0
    vcd: Optional[pathlib.Path] = None
    extra_sources: List[pathlib.Path] = dataclasses.field(default_factory=list)
    include_dirs: List[pathlib.Path] = dataclasses.field(default_factory=list)
    defines: Dict[str, str] = dataclasses.field(default_factory=dict)
    parameters: Dict[str, int] = dataclasses.field(default_factory=dict)


@dataclasses.dataclass
class GenericBackend(BackendSpec):
    name: str
    options: Dict[str, object] = dataclasses.field(default_factory=dict)


@dataclasses.dataclass
class ProgramSpec:
    name: str
    manifest_dir: pathlib.Path
    format: str = "memh"
    output: Optional[pathlib.Path] = None
    base_address: Optional[int] = None
    fill_value: int = 0
    listing: Optional[pathlib.Path] = None
    imem_output: Optional[pathlib.Path] = None
    segments: List[SegmentSpec] = dataclasses.field(default_factory=list)
    backends: List[BackendSpec] = dataclasses.field(default_factory=list)


@dataclasses.dataclass
class SegmentReport:
    label: str
    origin: int
    count: int
    min_addr: Optional[int]
    max_addr: Optional[int]


def _load_manifest(path: pathlib.Path) -> ProgramSpec:
    try:
        raw_text = path.read_text(encoding="utf-8")
    except FileNotFoundError as exc:
        raise ToolchainError(f"manifest '{path}' not found") from exc
    except OSError as exc:
        raise ToolchainError(f"unable to read manifest '{path}': {exc}") from exc

    if tomllib is not None:
        try:
            data = tomllib.loads(raw_text)
        except tomllib.TOMLDecodeError as exc:  # type: ignore[attr-defined]
            raise ToolchainError(f"manifest '{path}' is invalid TOML: {exc}") from exc
    else:
        data = _parse_manifest_text(raw_text, path)

    if "program" not in data:
        raise ToolchainError("manifest missing [program] section")
    program_tbl = data["program"]
    name = program_tbl.get("name", path.stem)
    fmt = program_tbl.get("format", "memh")
    if fmt not in {"memh", "hex", "bin"}:
        raise ToolchainError(f"[program].format must be 'memh', 'hex', or 'bin' (got '{fmt}')")

    manifest_dir = path.parent.resolve()
    output = program_tbl.get("output")
    if output is not None:
        output = manifest_dir.joinpath(output).resolve()
    listing = program_tbl.get("listing")
    if listing is not None:
        listing = manifest_dir.joinpath(listing).resolve()

    imem_output = program_tbl.get("imem_output")
    if imem_output is not None:
        if not isinstance(imem_output, str):
            raise ToolchainError("[program].imem_output must be a string path")
        imem_output = manifest_dir.joinpath(imem_output).resolve()
    elif output is not None and fmt == "memh":
        imem_output = output
    else:
        imem_output = None

    base_address = program_tbl.get("base_address")
    if base_address is not None and not isinstance(base_address, int):
        raise ToolchainError("[program].base_address must be an integer")
    fill = program_tbl.get("fill", 0)
    if not isinstance(fill, int):
        raise ToolchainError("[program].fill must be an integer")
    if fill < 0 or fill >= (1 << 48):
        raise ToolchainError("[program].fill must be within 0 <= fill < 2^48")

    segments_tbl: Optional[List[dict]] = None
    for key in ("segment", "segments"):
        candidate = data.get(key)
        if candidate is not None:
            segments_tbl = candidate
            break
    if not isinstance(segments_tbl, list) or not segments_tbl:
        raise ToolchainError("manifest must define one or more [[segment]] tables")

    segments: List[SegmentSpec] = []
    for idx, spec in enumerate(segments_tbl, start=1):
        if not isinstance(spec, dict):
            raise ToolchainError(f"segment #{idx} must be a table")
        if "source" not in spec:
            raise ToolchainError(f"segment #{idx} missing required 'source' field")
        source_value = spec["source"]
        if not isinstance(source_value, str):
            raise ToolchainError(f"segment #{idx} source must be a string path")
        src = manifest_dir.joinpath(source_value).resolve()
        origin = spec.get("origin", spec.get("base", 0))
        if not isinstance(origin, int):
            raise ToolchainError(f"segment #{idx} origin must be an integer")
        label = spec.get("label") or spec.get("name") or src.stem
        segments.append(SegmentSpec(path=src, origin=origin, label=str(label)))

    if not segments:
        raise ToolchainError("manifest must define at least one [[segment]] entry")

    backend_tbl = None
    for key in ("backend", "backends"):
        candidate = data.get(key)
        if candidate is not None:
            backend_tbl = candidate
            break
    if backend_tbl is None:
        backend_tbl = []
    if not isinstance(backend_tbl, list):
        raise ToolchainError("[[backend]] entries must form an array")

    backends: List[BackendSpec] = []
    for idx, spec in enumerate(backend_tbl, start=1):
        if not isinstance(spec, dict):
            raise ToolchainError(f"backend #{idx} must be a table")
        kind = spec.get("type") or spec.get("kind")
        if not kind:
            raise ToolchainError(f"backend #{idx} requires a 'type' field")
        enabled = spec.get("enable", spec.get("enabled", True))
        if not enabled:
            continue
        name_value = spec.get("name", f"{kind}{idx}")
        name_str = str(name_value)
        if kind == "simulation":
            backends.append(_parse_sim_backend(spec, manifest_dir, name_str))
        else:
            opts = {k: v for k, v in spec.items() if k not in {"type", "kind", "enable", "enabled"}}
            backends.append(GenericBackend(kind=str(kind), name=name_str, options=opts))

    return ProgramSpec(
        name=str(name),
        manifest_dir=manifest_dir,
        format=fmt,
        output=output,
        base_address=base_address,
        fill_value=fill,
        listing=listing,
        imem_output=imem_output,
        segments=segments,
        backends=backends,
    )


def _parse_sim_backend(spec: Dict[str, object], manifest_dir: pathlib.Path, name: str) -> SimulationSpec:
    def _as_path_list(value, field: str) -> List[pathlib.Path]:
        if isinstance(value, list):
            result: List[pathlib.Path] = []
            for item in value:
                if not isinstance(item, str):
                    raise ToolchainError(f"simulation backend '{name}': {field} entries must be strings")
                result.append(manifest_dir.joinpath(item).resolve())
            return result
        if isinstance(value, str):
            return [manifest_dir.joinpath(value).resolve()]
        raise ToolchainError(f"simulation backend '{name}': {field} must be a string or list of strings")

    top = spec.get("top") or spec.get("tb_top") or "run_core_tb"
    if not isinstance(top, str):
        raise ToolchainError(f"simulation backend '{name}': top must be a string")

    max_cycles_raw = spec.get("max_cycles", spec.get("cycles", 0))
    if max_cycles_raw is None:
        max_cycles_raw = 0
    if not isinstance(max_cycles_raw, int) or max_cycles_raw < 0:
        raise ToolchainError(f"simulation backend '{name}': max_cycles must be a non-negative integer")

    vcd_path = None
    vcd_raw = spec.get("vcd") or spec.get("waveform")
    if vcd_raw is not None:
        if not isinstance(vcd_raw, str):
            raise ToolchainError(f"simulation backend '{name}': vcd must be a string path")
        vcd_path = manifest_dir.joinpath(vcd_raw).resolve()

    extra_sources: List[pathlib.Path] = []
    for key in ("sources", "extra_sources", "rtl", "files"):
        if key in spec:
            extra_sources.extend(_as_path_list(spec[key], key))

    include_dirs: List[pathlib.Path] = []
    for key in ("include_dirs", "incdir"):
        if key in spec:
            include_dirs.extend(_as_path_list(spec[key], key))

    defines: Dict[str, str] = {}
    if "defines" in spec:
        defines_raw = spec["defines"]
        if isinstance(defines_raw, dict):
            for key, value in defines_raw.items():
                defines[str(key)] = str(value)
        elif isinstance(defines_raw, list):
            for entry in defines_raw:
                if isinstance(entry, str):
                    defines[entry] = "1"
                else:
                    raise ToolchainError(
                        f"simulation backend '{name}': defines list must contain only strings"
                    )
        else:
            raise ToolchainError(f"simulation backend '{name}': defines must be a table or list")

    parameters: Dict[str, int] = {}
    if "parameters" in spec:
        params = spec["parameters"]
        if not isinstance(params, dict):
            raise ToolchainError(f"simulation backend '{name}': parameters must be a table")
        for key, value in params.items():
            if not isinstance(value, int):
                raise ToolchainError(
                    f"simulation backend '{name}': parameter '{key}' must be an integer"
                )
            parameters[str(key)] = int(value)

    return SimulationSpec(
        kind="simulation",
        name=name,
        top=top,
        max_cycles=max_cycles_raw,
        vcd=vcd_path,
        extra_sources=extra_sources,
        include_dirs=include_dirs,
        defines=defines,
        parameters=parameters,
    )


def _assemble_segment(spec: SegmentSpec) -> Tuple[Dict[int, int], SegmentReport]:
    assembler = ad48_asm.Assembler()
    try:
        image = assembler.assemble(spec.path)
    except ad48_asm.AssemblerError as exc:
        raise ToolchainError(f"{spec.label}: {exc}") from exc
    if not image:
        return image, SegmentReport(spec.label, spec.origin, 0, None, None)
    min_addr = min(image)
    max_addr = max(image)
    return image, SegmentReport(spec.label, spec.origin, len(image), min_addr, max_addr)


def _merge_segments(program: ProgramSpec) -> Tuple[Dict[int, int], List[SegmentReport]]:
    merged: Dict[int, int] = {}
    occupancy: Dict[int, str] = {}
    reports: List[SegmentReport] = []

    for spec in program.segments:
        image, report = _assemble_segment(spec)
        reports.append(report)
        for addr, word in image.items():
            final_addr = addr + spec.origin
            if final_addr < 0:
                raise ToolchainError(
                    f"{spec.label}: address 0x{addr:X} + origin {spec.origin} yields negative location"
                )
            if final_addr in merged:
                other = occupancy[final_addr]
                raise ToolchainError(
                    f"address collision at 0x{final_addr:X}: segment '{spec.label}' overlaps '{other}'"
                )
            occupancy[final_addr] = spec.label
            merged[final_addr] = word & ((1 << 48) - 1)
    return merged, reports


def _format_hex(image: Dict[int, int], base_address: Optional[int], fill: int) -> str:
    if not image:
        return ""
    start = base_address if base_address is not None else min(image)
    if start < 0:
        raise ToolchainError("base address must be non-negative")
    end = max(image)
    lines: List[str] = []
    mask = (1 << 48) - 1
    for addr in range(start, end + 1):
        value = image.get(addr, fill) & mask
        lines.append(f"{value:012X}")
    return "\n".join(lines) + "\n"


def _write_listing(path: pathlib.Path, reports: List[SegmentReport]) -> None:
    lines: List[str] = ["# AD48 toolchain segment listing", ""]
    for report in reports:
        if report.count == 0:
            detail = "empty"
        else:
            offset_descr = f"origin 0x{report.origin:X}"
            start = f"0x{(report.min_addr or 0) + report.origin:X}"
            end = f"0x{(report.max_addr or 0) + report.origin:X}"
            detail = f"{start} - {end} ({offset_descr})"
        lines.append(f"- {report.label}: {report.count} word(s){' -> ' + detail if report.count else ''}")
    if not lines[-1]:
        lines.pop()
    content = "\n".join(lines) + "\n"
    path.parent.mkdir(parents=True, exist_ok=True)
    path.write_text(content, encoding="utf-8")


def _required_im_words(image: Dict[int, int]) -> int:
    if not image:
        return 1
    return max(image) + 1


def _format_cmd(cmd: List[str]) -> str:
    try:
        return shlex.join(cmd)
    except AttributeError:  # pragma: no cover - Python <3.8 fallback
        return " ".join(cmd)


def _plusarg_path(path: pathlib.Path) -> str:
    return str(path).replace("\\", "/")


def _resolve_sim_top_source(top: str) -> pathlib.Path:
    candidate = SIM_TESTBENCH_DIR / f"{top}.v"
    if candidate.exists():
        return candidate
    if top == DEFAULT_SIM_TESTBENCH.stem and DEFAULT_SIM_TESTBENCH.exists():
        return DEFAULT_SIM_TESTBENCH
    raise ToolchainError(
        f"simulation backend '{top}': unable to locate testbench source '{candidate}'. "
        "Provide the file via the manifest 'extra_sources' or place it under tools/sim."
    )


def _run_subprocess(cmd: List[str], *, cwd: pathlib.Path) -> None:
    try:
        subprocess.run(cmd, cwd=cwd, check=True)
    except FileNotFoundError as exc:
        raise ToolchainError(f"command not found: {cmd[0]}") from exc
    except subprocess.CalledProcessError as exc:
        raise ToolchainError(f"command failed ({exc.returncode}): {_format_cmd(cmd)}") from exc


def _run_simulation(
    program: ProgramSpec,
    backend: SimulationSpec,
    image: Dict[int, int],
    imem_path: pathlib.Path,
) -> None:
    if imem_path is None:
        raise ToolchainError("simulation requires an instruction memory output file")

    top_source = _resolve_sim_top_source(backend.top)

    build_root = program.manifest_dir / "build" / "sim" / backend.name
    build_root.mkdir(parents=True, exist_ok=True)
    sim_exe = build_root / f"{program.name}_{backend.name}.vvp"

    im_words = max(_required_im_words(image), 1)
    params = dict(backend.parameters)
    params.setdefault("IM_WORDS", max(im_words, 2))
    params.setdefault("DM_WORDS", 1024)

    include_dirs = list(dict.fromkeys(DEFAULT_SIM_INCLUDE_DIRS + backend.include_dirs))
    compile_cmd: List[str] = ["iverilog", "-g2012", "-o", str(sim_exe)]
    for inc in include_dirs:
        compile_cmd.extend(["-I", str(inc)])
    for define, value in backend.defines.items():
        if value == "" or value is None:
            compile_cmd.append(f"-D{define}")
        else:
            compile_cmd.append(f"-D{define}={value}")
    for param, value in params.items():
        compile_cmd.extend(["-P", f"{backend.top}.{param}={value}"])

    sources = [top_source] + DEFAULT_SIM_RTL + backend.extra_sources
    seen_sources = []
    for src in sources:
        if src in seen_sources:
            continue
        if not src.exists():
            raise ToolchainError(f"simulation backend '{backend.name}': missing source {src}")
        compile_cmd.append(str(src))
        seen_sources.append(src)

    print(f"[toolchain] compiling simulation '{backend.name}': {_format_cmd(compile_cmd)}", flush=True)
    _run_subprocess(compile_cmd, cwd=REPO_ROOT)

    run_cmd: List[str] = ["vvp", str(sim_exe), f"+IMEM={_plusarg_path(imem_path)}"]
    if backend.max_cycles:
        run_cmd.append(f"+MAX_CYCLES={backend.max_cycles}")
    if backend.vcd:
        backend.vcd.parent.mkdir(parents=True, exist_ok=True)
        run_cmd.append(f"+VCD={_plusarg_path(backend.vcd)}")

    print(f"[toolchain] running simulation '{backend.name}': {_format_cmd(run_cmd)}", flush=True)
    _run_subprocess(run_cmd, cwd=REPO_ROOT)


def parse_args(argv: Optional[List[str]] = None) -> argparse.Namespace:
    parser = argparse.ArgumentParser(
        description="AD48 assembler toolchain driver",
        formatter_class=argparse.ArgumentDefaultsHelpFormatter,
    )
    parser.add_argument("manifest", type=pathlib.Path, help="TOML manifest describing the ROM image")
    parser.add_argument("-o", "--output", type=pathlib.Path, help="Override the manifest output path")
    parser.add_argument("--format", choices=("memh", "hex", "bin"), help="Override the manifest output format")
    parser.add_argument(
        "--base-address",
        type=int,
        help="Override manifest base address used for contiguous hex exports",
    )
    parser.add_argument(
        "--fill",
        type=int,
        help="Override manifest fill value used for missing words in contiguous hex exports",
    )
    parser.add_argument(
        "--no-listing",
        action="store_true",
        help="Suppress listing generation even if manifest requests it",
    )
    parser.add_argument(
        "--imem-output",
        type=pathlib.Path,
        help="Override the instruction memory output path used for simulation",
    )
    parser.add_argument(
        "--run-sim",
        action="store_true",
        help="Compile and run the default simulation backend with Icarus Verilog",
    )
    parser.add_argument(
        "--sim-max-cycles",
        type=int,
        help="Override the simulation cycle limit (0 disables the timeout)",
    )
    parser.add_argument(
        "--sim-vcd",
        type=pathlib.Path,
        help="Emit a VCD waveform at the provided path during simulation",
    )
    parser.add_argument(
        "--sim-top",
        help="Override the simulation top-level module (default: run_core_tb)",
    )
    return parser.parse_args(argv)


def main(argv: Optional[List[str]] = None) -> int:
    args = parse_args(argv)
    try:
        program = _load_manifest(args.manifest.resolve())
        if args.format:
            program.format = args.format
        if args.output:
            program.output = args.output.resolve()
            if program.format == "memh":
                program.imem_output = program.output
        if args.base_address is not None:
            program.base_address = args.base_address
        if args.fill is not None:
            if args.fill < 0 or args.fill >= (1 << 48):
                raise ToolchainError("--fill must satisfy 0 <= fill < 2^48")
            program.fill_value = args.fill
        if args.imem_output:
            program.imem_output = args.imem_output.resolve()

        sim_backends = [b for b in program.backends if isinstance(b, SimulationSpec)]
        default_sim_top = DEFAULT_SIM_TESTBENCH.stem
        need_sim_backend = args.run_sim or args.sim_max_cycles is not None or args.sim_vcd is not None
        if need_sim_backend and not sim_backends:
            sim = SimulationSpec(
                kind="simulation",
                name="cli-sim",
                top=args.sim_top or default_sim_top,
            )
            sim_backends.append(sim)
            program.backends.append(sim)
        if args.sim_top:
            for sim in sim_backends:
                sim.top = args.sim_top

        if args.sim_max_cycles is not None:
            if args.sim_max_cycles < 0:
                raise ToolchainError("--sim-max-cycles must be non-negative")
            for sim in sim_backends:
                sim.max_cycles = args.sim_max_cycles

        if args.sim_vcd:
            vcd_path = args.sim_vcd.resolve()
            for sim in sim_backends:
                sim.vcd = vcd_path

        if sim_backends and program.imem_output is None:
            program.imem_output = (program.manifest_dir / f"{program.name}.memh").resolve()
        if program.imem_output and program.output is None and program.format == "memh":
            program.output = program.imem_output

        image, reports = _merge_segments(program)

        memh_text = ad48_asm.format_memh(image)
        imem_path = program.imem_output
        if imem_path:
            imem_path.parent.mkdir(parents=True, exist_ok=True)
            imem_path.write_text(memh_text, encoding="utf-8")

        if program.format == "memh":
            if program.output:
                if program.output != imem_path:
                    program.output.parent.mkdir(parents=True, exist_ok=True)
                    program.output.write_text(memh_text, encoding="utf-8")
            else:
                sys.stdout.write(memh_text)
        elif program.format == "hex":
            text = _format_hex(image, program.base_address, program.fill_value)
            if program.output:
                program.output.parent.mkdir(parents=True, exist_ok=True)
                program.output.write_text(text, encoding="utf-8")
            else:
                sys.stdout.write(text)
        else:  # bin
            data = ad48_asm.format_binary(image)
            if program.output:
                program.output.parent.mkdir(parents=True, exist_ok=True)
                program.output.write_bytes(data)
            else:
                sys.stdout.buffer.write(data)

        if program.listing and not args.no_listing:
            _write_listing(program.listing, reports)

        for backend in program.backends:
            if isinstance(backend, SimulationSpec):
                if imem_path is None:
                    raise ToolchainError("simulation requested but no instruction memory output was produced")
                _run_simulation(program, backend, image, imem_path)
            elif isinstance(backend, GenericBackend):
                print(f"[toolchain] backend '{backend.name}' ({backend.kind}) is not implemented yet; skipping")
            else:
                print(f"[toolchain] backend of type '{backend.kind}' is not recognised; skipping")
    except ToolchainError as exc:
        print(f"error: {exc}", file=sys.stderr)
        return 1
    return 0


if __name__ == "__main__":
    sys.exit(main())
