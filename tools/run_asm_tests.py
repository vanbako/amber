#!/usr/bin/env python3
"""Run all assembler-driven AD48 verification programs under simulation."""

from __future__ import annotations

import argparse
import pathlib
import subprocess
import sys
from typing import Iterable, List

REPO_ROOT = pathlib.Path(__file__).resolve().parent.parent
MANIFEST_DIR = REPO_ROOT / "verif" / "asm_tests"
TOOLCHAIN = REPO_ROOT / "tools" / "ad48_toolchain.py"


def discover_manifests(selected: Iterable[str] | None = None) -> List[pathlib.Path]:
    if selected:
        paths = []
        for entry in selected:
            candidate = pathlib.Path(entry)
            if not candidate.is_absolute():
                if len(candidate.parts) == 1:
                    candidate = MANIFEST_DIR / candidate
                else:
                    candidate = REPO_ROOT / candidate
            path = candidate.resolve()
            if path.is_file():
                paths.append(path)
            else:
                raise FileNotFoundError(f"manifest '{path}' not found")
        return paths
    return sorted(MANIFEST_DIR.glob("*.toml"))


def run_manifest(manifest: pathlib.Path) -> None:
    cmd = [sys.executable, str(TOOLCHAIN), str(manifest), "--run-sim"]
    print(f"[run_asm_tests] running {manifest.name} ...")
    subprocess.run(cmd, cwd=REPO_ROOT, check=True)


def main(argv: List[str] | None = None) -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument(
        "manifests",
        nargs="*",
        help="Optional specific manifest filenames (default: run all in verif/asm_tests)",
    )
    args = parser.parse_args(argv)

    manifests = discover_manifests(args.manifests or None)
    if not manifests:
        print("No manifests found under verif/asm_tests", file=sys.stderr)
        return 1

    for manifest in manifests:
        run_manifest(manifest)

    print("[run_asm_tests] all simulations completed successfully")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
