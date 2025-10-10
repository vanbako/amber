#!/usr/bin/env python3
"""Compile and run all AD48 Verilog testbenches with Icarus Verilog."""

from __future__ import annotations

import argparse
import dataclasses
import pathlib
import shutil
import subprocess
import sys
from typing import Iterable, List


REPO_ROOT = pathlib.Path(__file__).resolve().parent.parent
TB_DIR = REPO_ROOT / "verif" / "env"
RTL_SOURCES = [
    REPO_ROOT / "src" / "rtl" / "cpu_ad48.v",
    REPO_ROOT / "src" / "rtl" / "csr_unit.v",
    REPO_ROOT / "src" / "rtl" / "mem_access_unit.v",
    REPO_ROOT / "src" / "rtl" / "alu.v",
    REPO_ROOT / "src" / "rtl" / "regfiles.v",
    REPO_ROOT / "src" / "rtl" / "mem48.v",
    REPO_ROOT / "src" / "rtl" / "simple_soc_stub.v",
]
BUILD_ROOT = REPO_ROOT / "build" / "testbenches"


@dataclasses.dataclass(frozen=True)
class Testbench:
    path: pathlib.Path

    @property
    def name(self) -> str:
        return self.path.stem

    @property
    def build_dir(self) -> pathlib.Path:
        return BUILD_ROOT / self.name

    @property
    def exe_path(self) -> pathlib.Path:
        return self.build_dir / f"{self.name}.vvp"


def _check_tool(name: str, override: str | None = None) -> str:
    candidate = override or name
    resolved = shutil.which(candidate)
    if resolved is None:
        tool_msg = f"'{candidate}'" if override else f"'{name}'"
        raise SystemExit(f"[run_testbenches] error: {tool_msg} not found in PATH")
    return resolved


def discover_testbenches(selected: Iterable[str] | None) -> List[Testbench]:
    if selected:
        benches: List[Testbench] = []
        for entry in selected:
            candidate = pathlib.Path(entry)
            if not candidate.is_absolute():
                if len(candidate.parts) == 1:
                    if candidate.suffix:
                        candidate = TB_DIR / candidate
                    else:
                        candidate = TB_DIR / f"{candidate}.v"
                else:
                    candidate = (REPO_ROOT / candidate).resolve()
            path = candidate.resolve()
            if not path.is_file():
                raise FileNotFoundError(f"[run_testbenches] testbench '{path}' not found")
            benches.append(Testbench(path=path))
        return benches
    return [Testbench(path=p) for p in sorted(TB_DIR.glob("*_tb.v"))]


def compile_testbench(tb: Testbench, iverilog: str) -> pathlib.Path:
    tb.build_dir.mkdir(parents=True, exist_ok=True)
    cmd = [
        iverilog,
        "-g2012",
        "-o",
        str(tb.exe_path),
        "-s",
        tb.name,
        "-I",
        str(REPO_ROOT / "src" / "rtl"),
    ]
    cmd.extend(str(src) for src in RTL_SOURCES)
    cmd.append(str(tb.path))
    print(f"[run_testbenches] compiling {tb.name} ...")
    subprocess.run(cmd, cwd=REPO_ROOT, check=True)
    return tb.exe_path


def run_testbench(tb: Testbench, vvp: str) -> None:
    print(f"[run_testbenches] running {tb.name} ...")
    subprocess.run([vvp, str(tb.exe_path)], cwd=REPO_ROOT, check=True)


def main(argv: List[str] | None = None) -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument(
        "testbenches",
        nargs="*",
        help="Optional testbench filenames (default: run all under verif/env)",
    )
    parser.add_argument(
        "--iverilog",
        help="Override path to iverilog executable",
    )
    parser.add_argument(
        "--vvp",
        help="Override path to vvp executable",
    )
    args = parser.parse_args(argv)

    iverilog = _check_tool("iverilog", args.iverilog)
    vvp = _check_tool("vvp", args.vvp)

    try:
        benches = discover_testbenches(args.testbenches or None)
    except FileNotFoundError as exc:
        print(exc, file=sys.stderr)
        return 1

    if not benches:
        print("[run_testbenches] no testbenches found", file=sys.stderr)
        return 1

    BUILD_ROOT.mkdir(parents=True, exist_ok=True)

    failures = 0
    for tb in benches:
        try:
            compile_testbench(tb, iverilog)
        except subprocess.CalledProcessError as exc:
            failures += 1
            print(
                f"[run_testbenches] ERROR: iverilog failed for {tb.name} (exit {exc.returncode})",
                file=sys.stderr,
            )
            continue
        try:
            run_testbench(tb, vvp)
        except subprocess.CalledProcessError as exc:
            failures += 1
            print(
                f"[run_testbenches] ERROR: vvp failed for {tb.name} (exit {exc.returncode})",
                file=sys.stderr,
            )
            continue
        print(f"[run_testbenches] {tb.name} PASS")

    total = len(benches)
    passed = total - failures
    print(f"[run_testbenches] completed {total} testbench(es): {passed} passed, {failures} failed")
    return 0 if failures == 0 else 1


if __name__ == "__main__":
    raise SystemExit(main())
