#!/usr/bin/env python3
"""Assemble the Amber48 add_loop smoke test stub into instruction hex."""

from __future__ import annotations

import argparse
from dataclasses import dataclass
from pathlib import Path
from typing import Iterable, List, Tuple

# Architecture parameters for the 0.1 prototype
IMEM_DEPTH = 256
BAU_BYTES = 8

# Opcode map mirrored from amber48_decoder.sv
OPCODES = {
    "upper_imm": 0x00,
    "add": 0x10,
    "add_imm": 0x11,
    "sub": 0x12,
    "sub_imm": 0x13,
    "and": 0x20,
    "or": 0x21,
    "xor": 0x22,
    "xor_imm": 0x23,
    "lsl": 0x30,
    "lsr": 0x31,
    "branch_eq": 0x40,
    "branch_ne": 0x41,
    "branch_ltu": 0x42,
    "branch_lts": 0x43,
    "branch_gtu": 0x44,
    "branch_gts": 0x45,
    "branch_zero": 0x46,
    "branch_notzero": 0x47,
    "branch_always": 0x48,
    "load": 0x60,
    "store": 0x61,
}

REGISTER_ALIASES = {
    "sp": 13,
    "lr": 14,
    "ssp": 15,
}

COMMENT_SENTINELS = ("#", "//")


class AssemblerError(RuntimeError):
    """Wrap user-visible assembly issues."""


@dataclass
class Instruction:
    mnemonic: str
    args: Tuple[str, ...]
    pc: int
    line_num: int
    source: str


def strip_comment(line: str) -> str:
    """Remove comments while respecting single-quoted character literals."""
    if not line:
        return line
    stripped = []
    in_quote = False
    idx = 0
    while idx < len(line):
        ch = line[idx]
        if ch == "'":
            prev_escape = idx > 0 and line[idx - 1] == "\\"
            if not prev_escape:
                in_quote = not in_quote
        if not in_quote:
            for marker in COMMENT_SENTINELS:
                if line.startswith(marker, idx):
                    return "".join(stripped)
        stripped.append(ch)
        idx += 1
    return "".join(stripped)


def parse_register(token: str, line_num: int) -> int:
    token = token.strip().lower()
    if token in REGISTER_ALIASES:
        return REGISTER_ALIASES[token]
    if token.startswith("r") and token[1:].isdigit():
        value = int(token[1:])
        if 0 <= value <= 15:
            return value
    raise AssemblerError(f"Line {line_num}: invalid register '{token}'")


def parse_immediate(token: str, line_num: int) -> int:
    token = token.strip()
    if not token:
        raise AssemblerError(f"Line {line_num}: missing immediate value")
    if token.startswith("'") and token.endswith("'") and len(token) >= 3:
        body = token[1:-1]
        if body == "\\n":
            return 10
        if len(body) == 1:
            return ord(body)
        raise AssemblerError(f"Line {line_num}: unsupported character literal {token}")
    try:
        return int(token, 0)
    except ValueError as exc:
        raise AssemblerError(f"Line {line_num}: invalid immediate '{token}'") from exc


def encode_imm16(value: int, line_num: int) -> int:
    if value < -(1 << 15) or value >= (1 << 15):
        raise AssemblerError(f"Line {line_num}: immediate {value} exceeds 16-bit signed range")
    return value & 0xFFFF


def pack_instruction(opcode: int, imm: int, rs1: int, rs2: int, rd: int) -> int:
    word = (opcode & 0xFF) << 40
    word |= (imm & 0xFFFF) << 24
    word |= (rs1 & 0xF) << 20
    word |= (rs2 & 0xF) << 16
    word |= (rd & 0xF) << 12
    return word


def parse_program(text: str) -> Tuple[List[Instruction], dict[str, int]]:
    instructions: List[Instruction] = []
    labels: dict[str, int] = {}
    pc = 0
    for line_num, raw_line in enumerate(text.splitlines(), start=1):
        without_comment = strip_comment(raw_line)
        stripped = without_comment.strip()
        if not stripped:
            continue
        if stripped.endswith(":"):
            label = stripped[:-1].strip()
            if not label:
                raise AssemblerError(f"Line {line_num}: empty label declaration")
            label_key = label.lower()
            if label_key in labels:
                raise AssemblerError(f"Line {line_num}: duplicate label '{label}'")
            labels[label_key] = pc
            continue
        parts = stripped.split(None, 1)
        mnemonic = parts[0].lower()
        arg_text = parts[1] if len(parts) > 1 else ""
        args = tuple(arg.strip() for arg in arg_text.split(",") if arg.strip())
        instructions.append(Instruction(mnemonic, args, pc, line_num, stripped))
        pc += BAU_BYTES
    return instructions, labels


def resolve_label(label: str, labels: dict[str, int], line_num: int) -> int:
    key = label.lower()
    if key not in labels:
        raise AssemblerError(f"Line {line_num}: unknown label '{label}'")
    return labels[key]


def assemble_instruction(inst: Instruction, labels: dict[str, int]) -> int:
    mnem = inst.mnemonic
    if mnem == "nop":
        return pack_instruction(OPCODES["add_imm"], encode_imm16(0, inst.line_num), 0, 0, 0)
    if mnem == "halt":
        return 0xFF0000000000
    if mnem not in OPCODES:
        raise AssemblerError(f"Line {inst.line_num}: unsupported mnemonic '{mnem}'")

    opcode = OPCODES[mnem]

    if mnem == "add_imm":
        if len(inst.args) != 3:
            raise AssemblerError(f"Line {inst.line_num}: add_imm expects rd, rs1, imm")
        rd = parse_register(inst.args[0], inst.line_num)
        rs1 = parse_register(inst.args[1], inst.line_num)
        imm = encode_imm16(parse_immediate(inst.args[2], inst.line_num), inst.line_num)
        return pack_instruction(opcode, imm, rs1, 0, rd)

    if mnem == "add":
        if len(inst.args) != 3:
            raise AssemblerError(f"Line {inst.line_num}: add expects rd, rs1, rs2")
        rd = parse_register(inst.args[0], inst.line_num)
        rs1 = parse_register(inst.args[1], inst.line_num)
        rs2 = parse_register(inst.args[2], inst.line_num)
        return pack_instruction(opcode, encode_imm16(0, inst.line_num), rs1, rs2, rd)

    if mnem in {"branch_eq", "branch_ne", "branch_ltu", "branch_lts", "branch_gtu", "branch_gts"}:
        if len(inst.args) != 3:
            raise AssemblerError(f"Line {inst.line_num}: {mnem} expects rs1, rs2, label")
        rs1 = parse_register(inst.args[0], inst.line_num)
        rs2 = parse_register(inst.args[1], inst.line_num)
        target_pc = resolve_label(inst.args[2], labels, inst.line_num)
        offset = target_pc - inst.pc
        imm = encode_imm16(offset, inst.line_num)
        return pack_instruction(opcode, imm, rs1, rs2, 0)

    if mnem == "branch_always":
        if len(inst.args) != 1:
            raise AssemblerError(f"Line {inst.line_num}: branch_always expects a label")
        target_pc = resolve_label(inst.args[0], labels, inst.line_num)
        offset = target_pc - inst.pc
        imm = encode_imm16(offset, inst.line_num)
        return pack_instruction(opcode, imm, 0, 0, 0)

    if mnem == "load":
        if len(inst.args) != 2:
            raise AssemblerError(f"Line {inst.line_num}: load expects rd, offset(base)")
        rd = parse_register(inst.args[0], inst.line_num)
        imm, base = parse_memory_operand(inst.args[1], inst.line_num)
        if base not in (0,):
            raise AssemblerError(f"Line {inst.line_num}: load currently only supports r0 as base")
        return pack_instruction(opcode, encode_imm16(imm, inst.line_num), base, 0, rd)

    if mnem == "store":
        if len(inst.args) != 2:
            raise AssemblerError(f"Line {inst.line_num}: store expects rs, offset(base)")
        rs2 = parse_register(inst.args[0], inst.line_num)
        imm, base = parse_memory_operand(inst.args[1], inst.line_num)
        if base not in (0,):
            raise AssemblerError(f"Line {inst.line_num}: store currently only supports r0 as base")
        return pack_instruction(opcode, encode_imm16(imm, inst.line_num), base, rs2, 0)

    raise AssemblerError(f"Line {inst.line_num}: mnemonic '{mnem}' is not yet implemented")


def parse_memory_operand(token: str, line_num: int) -> Tuple[int, int]:
    token = token.strip()
    if not token:
        raise AssemblerError(f"Line {line_num}: missing memory operand")
    if token.endswith(")") and "(" in token:
        offset_part, reg_part = token.split("(", 1)
        reg_token = reg_part.rstrip(")")
        imm = parse_immediate(offset_part or "0", line_num)
        base = parse_register(reg_token, line_num)
        return imm, base
    # Bare immediate
    imm = parse_immediate(token, line_num)
    return imm, 0


def assemble(text: str) -> List[int]:
    instructions, labels = parse_program(text)
    words: List[int] = []
    for inst in instructions:
        word = assemble_instruction(inst, labels)
        words.append(word)
    if len(words) > IMEM_DEPTH:
        raise AssemblerError(
            f"Program uses {len(words)} instructions, exceeds IMEM depth {IMEM_DEPTH}")
    return words


def write_hex(words: Iterable[int], path: Path) -> None:
    rows = list(words)
    if len(rows) < IMEM_DEPTH:
        rows.extend(0 for _ in range(IMEM_DEPTH - len(rows)))
    with path.open("w", encoding="ascii") as handle:
        for value in rows:
            handle.write(f"{value:012x}\n")


def main() -> None:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument(
        "stub",
        type=Path,
        nargs="?",
        default=Path("tests/add_loop.s"),
        help="Path to the assembly stub (default: tests/add_loop.s)",
    )
    parser.add_argument(
        "-o",
        "--output",
        type=Path,
        default=Path("build/amber48_smoke.hex"),
        help="Target hex file path (default: build/amber48_smoke.hex)",
    )
    args = parser.parse_args()

    text = args.stub.read_text(encoding="ascii")
    words = assemble(text)
    args.output.parent.mkdir(parents=True, exist_ok=True)
    write_hex(words, args.output)


if __name__ == "__main__":
    try:
        main()
    except AssemblerError as err:
        raise SystemExit(str(err))
