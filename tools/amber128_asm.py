#!/usr/bin/env python3
"""
Minimal assembler for amber128 (0.2) 24-bit single-slot instructions packed into
128-bit bundles. Supports:

- br imm                      ; PC-relative, imm in bundles (16B units)
- beq rd, rs, imm             ; rs must be r0..r7
- bne rd, rs, imm             ; rs must be r0..r7
- ld128 cD, [cA + imm]
- st128 [cA + imm], cD
- li rd, imm16
- capmov cD, rN
- 12-bit ALU ops auto-packed per slot:
  nop
  add rd, rs | sub rd, rs | and rd, rs | or rd, rs | xor rd, rs
  lsl rd, rs | lsr rd, rs | mov rd, rs

Program format:
- One instruction per line; blank line starts a new bundle.
- Up to 5 slots per bundle. 12-bit instructions are packed two-per-slot with
  the slot flag bit set; a lone 12-bit op is padded with a trailing nop.
- Labels as `name:` on a line. Branch immediates may use labels; imm resolved as
  (target_bundle - current_bundle).

Outputs a ROM hex with one 128-bit word per line (32 hex chars).
"""

from __future__ import annotations

import argparse
from dataclasses import dataclass
from pathlib import Path
from typing import Dict, List, Tuple

OPC_BR = 0x8
OPC_BEQ = 0x9
OPC_BNE = 0xA
OPC_LD = 0x1
OPC_ST = 0x2
OPC_LI = 0x3
OPC_CAPMOVE = 0x4

TWELVE_OPS: Dict[str, int] = {
    "nop": 0x0,
    "add": 0x1,
    "sub": 0x2,
    "and": 0x3,
    "or": 0x4,
    "xor": 0x5,
    "lsl": 0x6,
    "lsr": 0x7,
    "mov": 0x8,
}

TWENTYFOUR_OPS = {"br", "beq", "bne", "ld128", "st128", "li", "capmov"}
SUPPORTED_OPS = TWENTYFOUR_OPS | set(TWELVE_OPS.keys())


@dataclass
class Inst:
    kind: str
    args: Tuple[str, ...]
    line: int
    bundle_index: int
    is_12bit: bool


@dataclass
class Encoded:
    width: int  # 12 or 24
    value: int


def strip_comment(line: str) -> str:
    if not line:
        return line
    s = line
    q = False
    out = []
    i = 0
    while i < len(s):
        ch = s[i]
        if ch == '"':
            q = not q
            out.append(ch)
            i += 1
            continue
        if not q:
            if s.startswith('//', i):
                break
            if ch == '#':
                break
        out.append(ch)
        i += 1
    return ''.join(out).strip()


def parse_reg(token: str, line: int) -> int:
    t = token.strip().lower()
    if t.startswith('r') and t[1:].isdigit():
        v = int(t[1:])
        if 0 <= v <= 15:
            return v
    raise ValueError(f"Line {line}: invalid register '{token}' (expected r0..r15)")


def parse_reg3(token: str, line: int) -> int:
    v = parse_reg(token, line)
    if v > 7:
        raise ValueError(f"Line {line}: 12-bit ops require r0..r7, got '{token}'")
    return v


def parse_creg(token: str, line: int) -> int:
    t = token.strip().lower()
    if t.startswith('c') and t[1:].isdigit():
        v = int(t[1:])
        if 0 <= v <= 7:
            return v
    raise ValueError(f"Line {line}: invalid capability '{token}' (expected c0..c7)")


def parse_imm(token: str, line: int) -> int:
    t = token.strip()
    base = 10
    if t.startswith('0x') or t.startswith('-0x'):
        base = 16
    try:
        val = int(t, base)
    except ValueError as exc:
        raise ValueError(f"Line {line}: invalid immediate '{token}'") from exc
    if val < -(1 << 12) or val >= (1 << 12):
        raise ValueError(f"Line {line}: immediate {val} does not fit 13-bit signed")
    return val


def parse_imm16(token: str, line: int) -> int:
    t = token.strip()
    base = 10
    if t.startswith('0x') or t.startswith('-0x'):
        base = 16
    try:
        val = int(t, base)
    except ValueError as exc:
        raise ValueError(f"Line {line}: invalid 16-bit immediate '{token}'") from exc
    if val < -(1 << 15) or val >= (1 << 15):
        raise ValueError(f"Line {line}: immediate {val} does not fit 16-bit signed")
    return val


def enc_slot24(opc4: int, rd4: int, cap3: int, imm13: int) -> int:
    value = ((opc4 & 0xF) << 20) | ((rd4 & 0xF) << 16) | ((cap3 & 0x7) << 13) | (imm13 & 0x1FFF)
    return value & 0xFFFFFF


def enc_slot12(opc4: int, rd3: int, rs3: int) -> int:
    value = ((opc4 & 0xF) << 8) | ((rd3 & 0x7) << 5) | ((rs3 & 0x7) << 2)
    return value & 0xFFF


def encode_inst(inst: Inst, labels: Dict[str, int]) -> Encoded:
    k = inst.kind
    args = inst.args
    if k in TWELVE_OPS:
        opc = TWELVE_OPS[k]
        if k == 'nop':
            if args:
                raise ValueError(f"Line {inst.line}: nop takes no operands")
            rd = 0
            rs = 0
        elif k == 'mov':
            if len(args) != 2:
                raise ValueError(f"Line {inst.line}: mov expects rd, rs")
            rd = parse_reg3(args[0], inst.line)
            rs = parse_reg3(args[1], inst.line)
        else:
            if len(args) != 2:
                raise ValueError(f"Line {inst.line}: {k} expects rd, rs")
            rd = parse_reg3(args[0], inst.line)
            rs = parse_reg3(args[1], inst.line)
        return Encoded(12, enc_slot12(opc, rd, rs))
    if k == 'br':
        if len(args) != 1:
            raise ValueError(f"Line {inst.line}: br expects imm or label")
        imm = parse_imm_or_label(args[0], labels, inst)
        return Encoded(24, enc_slot24(OPC_BR, 0, 0, imm))
    if k in ('beq', 'bne'):
        if len(args) != 3:
            raise ValueError(f"Line {inst.line}: {k} expects rd, rs, imm/label")
        rd = parse_reg(args[0], inst.line)
        rs = parse_reg(args[1], inst.line)
        if rs > 7:
            raise ValueError(f"Line {inst.line}: rs must be r0..r7 for {k}")
        imm = parse_imm_or_label(args[2], labels, inst)
        opc = OPC_BEQ if k == 'beq' else OPC_BNE
        return Encoded(24, enc_slot24(opc, rd, rs, imm))
    if k == 'li':
        if len(args) != 2:
            raise ValueError(f"Line {inst.line}: li expects rd, imm16")
        rd = parse_reg(args[0], inst.line)
        imm = parse_imm16(args[1], inst.line)
        value = imm & 0xFFFF
        cap_bits = (value >> 13) & 0x7
        imm_bits = value & 0x1FFF
        return Encoded(24, enc_slot24(OPC_LI, rd, cap_bits, imm_bits))
    if k == 'capmov':
        if len(args) != 2:
            raise ValueError(f"Line {inst.line}: capmov expects cD, rN")
        cd = parse_creg(args[0], inst.line)
        rd = parse_reg(args[1], inst.line)
        if rd >= 15:
            raise ValueError(f"Line {inst.line}: capmov requires r0..r14 so rd+1 is valid")
        return Encoded(24, enc_slot24(OPC_CAPMOVE, rd, cd, 0))
    if k == 'ld128':
        if len(args) != 2:
            raise ValueError(f"Line {inst.line}: ld128 expects cD, [cA + imm]")
        cd = parse_creg(args[0], inst.line)
        cap, imm = parse_memcap(args[1], inst.line)
        return Encoded(24, enc_slot24(OPC_LD, cd, cap, imm))
    if k == 'st128':
        if len(args) != 2:
            raise ValueError(f"Line {inst.line}: st128 expects [cA + imm], cD")
        cap, imm = parse_memcap(args[0], inst.line)
        cd = parse_creg(args[1], inst.line)
        return Encoded(24, enc_slot24(OPC_ST, cd, cap, imm))
    raise ValueError(f"Line {inst.line}: unknown mnemonic '{k}'")


def parse_memcap(token: str, line: int) -> Tuple[int, int]:
    t = token.strip().lower()
    if not (t.startswith('[') and t.endswith(']')):
        raise ValueError(f"Line {line}: expected [cA + imm], got '{token}'")
    inner = t[1:-1].strip()
    if '+' in inner:
        left, right = inner.split('+', 1)
        cap = parse_creg(left.strip(), line)
        imm = parse_imm(right.strip(), line)
    else:
        cap = parse_creg(inner.strip(), line)
        imm = 0
    return cap, imm


def parse_imm_or_label(tok: str, labels: Dict[str, int], inst: Inst) -> int:
    t = tok.strip()
    if t.endswith(':'):
        t = t[:-1]
    key = t.lower()
    if key in labels:
        target_bundle = labels[key]
        delta = target_bundle - inst.bundle_index
        return delta
    return parse_imm(t, inst.line)


def parse_program(text: str) -> Tuple[List[Inst], Dict[str, int]]:
    instructions: List[Inst] = []
    labels: Dict[str, int] = {}
    bundle_index = 0
    slot_idx = 0
    half_slot = 0
    bundle_used = False

    for ln, raw in enumerate(text.splitlines(), start=1):
        s = strip_comment(raw)
        if not s:
            if bundle_used or slot_idx != 0 or half_slot != 0:
                bundle_index += 1
                slot_idx = 0
                half_slot = 0
                bundle_used = False
            continue
        if s.endswith(':') and s[:-1].strip():
            label = s[:-1].strip().lower()
            if label in labels:
                raise ValueError(f"Line {ln}: duplicate label '{label}'")
            labels[label] = bundle_index
            continue
        parts = s.split(None, 1)
        mnem = parts[0].lower()
        if mnem not in SUPPORTED_OPS:
            raise ValueError(f"Line {ln}: unsupported mnemonic '{mnem}'")
        arg_text = parts[1] if len(parts) > 1 else ''
        args = tuple(a.strip() for a in arg_text.split(',') if a.strip())
        is_12 = mnem in TWELVE_OPS
        if not is_12 and half_slot:
            half_slot = 0
            slot_idx += 1
            if slot_idx == 5:
                bundle_index += 1
                slot_idx = 0
                bundle_used = False
        instructions.append(Inst(mnem, args, ln, bundle_index, is_12))
        bundle_used = True
        if is_12:
            if half_slot:
                half_slot = 0
                slot_idx += 1
                if slot_idx == 5:
                    bundle_index += 1
                    slot_idx = 0
                    bundle_used = False
            else:
                half_slot = 1
        else:
            slot_idx += 1
            if slot_idx == 5:
                bundle_index += 1
                slot_idx = 0
                bundle_used = False

    return instructions, labels


def build_bundles(instructions: List[Inst], labels: Dict[str, int]) -> List[int]:
    bundles: List[int] = []
    current_slots: List[int] = []
    current_flags = 0
    pending_half: int | None = None

    def flush_current_bundle() -> None:
        nonlocal current_slots, current_flags, pending_half, bundles
        if pending_half is not None:
            slot_index = len(current_slots)
            current_flags |= 1 << (4 - slot_index)
            current_slots.append((pending_half & 0xFFF) << 12)
            pending_half = None
        if not current_slots and pending_half is None:
            current_slots = [0] * 5
            bundles.append(pack_bundle(0, current_slots))
        else:
            while len(current_slots) < 5:
                current_slots.append(0)
            bundles.append(pack_bundle(current_flags, current_slots))
        current_slots = []
        current_flags = 0

    for inst in instructions:
        while inst.bundle_index > len(bundles):
            flush_current_bundle()
        encoded = encode_inst(inst, labels)
        if encoded.width == 24:
            if pending_half is not None:
                slot_index = len(current_slots)
                current_flags |= 1 << (4 - slot_index)
                current_slots.append((pending_half & 0xFFF) << 12)
                pending_half = None
                if len(current_slots) == 5:
                    flush_current_bundle()
                    continue
            current_slots.append(encoded.value & 0xFFFFFF)
            if len(current_slots) == 5:
                flush_current_bundle()
        else:
            if pending_half is None:
                pending_half = encoded.value & 0xFFF
            else:
                slot_index = len(current_slots)
                packed = ((pending_half & 0xFFF) << 12) | (encoded.value & 0xFFF)
                current_flags |= 1 << (4 - slot_index)
                current_slots.append(packed)
                pending_half = None
                if len(current_slots) == 5:
                    flush_current_bundle()

    if pending_half is not None or current_slots:
        flush_current_bundle()

    return bundles


def pack_bundle(flags5: int, slots24: List[int]) -> int:
    assert len(slots24) == 5
    word = (flags5 & 0x1F) << 123
    bit = 96
    for slot in slots24:
        word |= (slot & 0xFFFFFF) << bit
        bit -= 24
    return word


def write_hex(words: List[int], path: Path) -> None:
    path.parent.mkdir(parents=True, exist_ok=True)
    with path.open('w', encoding='ascii') as fh:
        for w in words:
            fh.write(f"{w:032x}\n")


def main() -> None:
    ap = argparse.ArgumentParser(description=__doc__)
    ap.add_argument('input', type=Path, help='Input .s/.asm file')
    ap.add_argument('-o', '--output', type=Path, default=Path('build/amber128_prog.hex'), help='Output ROM hex path')
    args = ap.parse_args()

    text = args.input.read_text(encoding='ascii')
    insts, labels = parse_program(text)
    bundles = build_bundles(insts, labels)
    write_hex(bundles, args.output)


if __name__ == '__main__':
    main()
