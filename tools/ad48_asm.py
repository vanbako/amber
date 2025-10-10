#!/usr/bin/env python3
"""AD48 macro assembler.

Parses assembly for the Amber AD48 CPU, supporting user macros, labels,
constants, and multiple output formats.
"""

from __future__ import annotations

import argparse
import ast
import dataclasses
import itertools
import pathlib
import re
import sys
from typing import Callable, Dict, Iterable, List, Optional, Sequence, Tuple


class AssemblerError(RuntimeError):
    """Raised when assembly fails."""
    pass


@dataclasses.dataclass(frozen=True)
class SourceLocation:
    path: pathlib.Path
    line: int

    def __str__(self) -> str:
        return f"{self.path}:{self.line}"


@dataclasses.dataclass
class SourceLine:
    loc: SourceLocation
    text: str


@dataclasses.dataclass
class MacroDefinition:
    name: str
    params: List[str]
    variadic: Optional[str]
    body: List[SourceLine]


def strip_comment(text: str) -> str:
    """Remove inline comments introduced by //, #, or ;."""
    positions = [
        pos for marker in ("//", "#", ";")
        if (pos := text.find(marker)) != -1
    ]
    if positions:
        return text[: min(positions)]
    return text


LABEL_RE = re.compile(r"\s*([A-Za-z_][\w@$\.]*)\s*:\s*")


def extract_labels(text: str) -> Tuple[List[str], str]:
    labels: List[str] = []
    idx = 0
    while True:
        match = LABEL_RE.match(text, idx)
        if not match:
            break
        labels.append(match.group(1))
        idx = match.end()
    remainder = text[idx:].strip()
    return labels, remainder


def split_args(arg_string: str) -> List[str]:
    args: List[str] = []
    buf: List[str] = []
    depth = 0
    for ch in arg_string:
        if ch in "([{":
            depth += 1
        elif ch in ")]}":
            depth = max(depth - 1, 0)
        elif ch == "," and depth == 0:
            arg = "".join(buf).strip()
            if arg:
                args.append(arg)
            buf.clear()
            continue
        buf.append(ch)
    trailing = "".join(buf).strip()
    if trailing:
        args.append(trailing)
    return args


def tokenize_params(spec: str) -> List[str]:
    if not spec.strip():
        return []
    return [part for part in re.split(r"[,\s]+", spec.strip()) if part]


def is_identifier(token: str) -> bool:
    return bool(re.fullmatch(r"[A-Za-z_][\w@$\.]*", token))


def read_source(path: pathlib.Path) -> List[SourceLine]:
    lines: List[SourceLine] = []
    with path.open("r", encoding="utf-8") as fh:
        for idx, raw in enumerate(fh, start=1):
            lines.append(SourceLine(SourceLocation(path, idx), raw.rstrip("\n")))
    return lines


REGISTER_RE = re.compile(r"([aAdD])([0-7])$")
SPECIAL_REGISTERS = {
    "ssp": ("A", 7),
}


def parse_register(token: str, *, want_bank: Optional[str] = None, loc: Optional[SourceLocation] = None) -> "Register":
    token = token.strip()
    alias = token.lower()
    if alias in SPECIAL_REGISTERS:
        bank, idx = SPECIAL_REGISTERS[alias]
        if want_bank and want_bank.upper() != bank:
            msg = f"expected {want_bank}-bank register, got '{token}'"
            if loc:
                raise AssemblerError(f"{loc}: {msg}")
            raise AssemblerError(msg)
        return Register(bank=bank, index=idx, token=token)
    match = REGISTER_RE.fullmatch(token)
    if not match:
        msg = f"invalid register '{token}'"
        if loc:
            raise AssemblerError(f"{loc}: {msg}")
        raise AssemblerError(msg)
    bank = match.group(1).upper()
    idx = int(match.group(2))
    if want_bank and bank != want_bank:
        msg = f"expected {want_bank}-bank register, got '{token}'"
        if loc:
            raise AssemblerError(f"{loc}: {msg}")
        raise AssemblerError(msg)
    return Register(bank=bank, index=idx, token=token)


def is_register_token(token: str) -> bool:
    stripped = token.strip()
    return bool(REGISTER_RE.fullmatch(stripped)) or stripped.lower() in SPECIAL_REGISTERS


@dataclasses.dataclass(frozen=True)
class Register:
    bank: str  # 'A' or 'D'
    index: int
    token: str

    @property
    def rd_bank(self) -> int:
        return 0 if self.bank == "A" else 1


def collect_macros(lines: Sequence[SourceLine]) -> Tuple[Dict[str, MacroDefinition], List[SourceLine]]:
    macros: Dict[str, MacroDefinition] = {}
    output: List[SourceLine] = []
    i = 0
    total = len(lines)
    while i < total:
        line = lines[i]
        stripped = strip_comment(line.text).strip()
        labels, remainder = extract_labels(stripped)
        for lbl in labels:
            output.append(SourceLine(line.loc, f"{lbl}:"))
        stripped = remainder
        if not stripped:
            i += 1
            continue
        token = stripped.split(maxsplit=1)[0].lower()
        if token != ".macro":
            output.append(SourceLine(line.loc, stripped))
            i += 1
            continue
        rest = stripped[len(token):].strip()
        if not rest:
            raise AssemblerError(f"{line.loc}: .macro requires a name")
        parts = rest.split(None, 1)
        name = parts[0].lower()
        if not is_identifier(name):
            raise AssemblerError(f"{line.loc}: invalid macro name '{name}'")
        param_spec = parts[1] if len(parts) > 1 else ""
        params = tokenize_params(param_spec)
        variadic = None
        if params and params[-1].endswith("..."):
            variadic = params[-1].rstrip(".") or "args"
            params = params[:-1]
        body: List[SourceLine] = []
        i += 1
        while i < total:
            body_line = lines[i]
            body_text = strip_comment(body_line.text).rstrip()
            if body_text.strip().lower() == ".endmacro":
                break
            body.append(SourceLine(body_line.loc, body_text))
            i += 1
        else:
            raise AssemblerError(f"{line.loc}: unterminated .macro {name}")
        macros[name] = MacroDefinition(name=name, params=params, variadic=variadic, body=body)
        i += 1  # skip .endmacro
    return macros, output


class MacroExpander:
    def __init__(self, macros: Dict[str, MacroDefinition]):
        self.macros = macros
        self._unique = itertools.count()

    def expand(self, lines: Sequence[SourceLine]) -> List[SourceLine]:
        return self._expand_lines(lines, ())

    def _expand_lines(self, lines: Sequence[SourceLine], stack: Tuple[str, ...]) -> List[SourceLine]:
        result: List[SourceLine] = []
        for line in lines:
            stripped = strip_comment(line.text).strip()
            if not stripped:
                continue
            labels, remainder = extract_labels(stripped)
            for lbl in labels:
                result.append(SourceLine(line.loc, f"{lbl}:"))
            if not remainder:
                continue
            token = remainder.split(maxsplit=1)[0].lower()
            if token in self.macros:
                macro = self.macros[token]
                args = split_args(remainder[len(token):].strip())
                result.extend(self._expand_macro(line.loc, macro, args, stack))
            else:
                result.append(SourceLine(line.loc, remainder))
        return result

    def _expand_macro(
        self,
        call_loc: SourceLocation,
        macro: MacroDefinition,
        args: Sequence[str],
        stack: Tuple[str, ...],
    ) -> List[SourceLine]:
        if macro.name in stack:
            chain = " -> ".join(stack + (macro.name,))
            raise AssemblerError(f"{call_loc}: recursive macro expansion ({chain})")
        if macro.variadic:
            fixed = len(macro.params)
            if len(args) < fixed:
                raise AssemblerError(
                    f"{call_loc}: macro '{macro.name}' expects at least {fixed} args, got {len(args)}"
                )
            mapping = {p: a for p, a in zip(macro.params, args[:fixed])}
            mapping[macro.variadic] = ", ".join(args[fixed:])
        else:
            if len(args) != len(macro.params):
                raise AssemblerError(
                    f"{call_loc}: macro '{macro.name}' expects {len(macro.params)} args, got {len(args)}"
                )
            mapping = {p: a for p, a in zip(macro.params, args)}
        uid = f"__mac{next(self._unique)}"
        pattern = re.compile(r"\\([A-Za-z_][\w@\.]*|@)")

        def substitute(text: str) -> str:
            def repl(match: re.Match[str]) -> str:
                key = match.group(1)
                if key == "@":
                    return uid
                if key in mapping:
                    return mapping[key]
                raise AssemblerError(f"{call_loc}: macro '{macro.name}' placeholder '\\{key}' undefined")

            return pattern.sub(repl, text)

        substituted = [
            SourceLine(line.loc, substitute(line.text)) for line in macro.body
        ]
        return self._expand_lines(substituted, stack + (macro.name,))

OP_ALU = 0x0
OP_ALUI_A = 0x1
OP_ALUI_D = 0x2
OP_LD = 0x3
OP_ST = 0x4
OP_BR = 0x5
OP_JAL = 0x6
OP_JALR = 0x7
OP_CSR = 0x8
OP_SYS = 0xF

F_ADD = 0x0
F_SUB = 0x1
F_AND = 0x2
F_OR = 0x3
F_XOR = 0x4
F_SLL = 0x5
F_SRL = 0x6
F_SRA = 0x7
F_NOT = 0x8

C_BEQ = 0b000
C_BNE = 0b001
C_BLT = 0b010
C_BLTU = 0b011
C_BGE = 0b100
C_BGEU = 0b101
C_ALWAYS = 0b111

CSR_F_RW = 0x0
CSR_F_RS = 0x1
CSR_F_RC = 0x2
CSR_F_R = 0x3

CSR_NAME_TO_ADDR = {
    "status": 0x000,
    "scratch": 0x001,
    "epc": 0x002,
    "cause": 0x003,
    "lr": 0x004,
    "ssp": 0x005,
    "irq_enable": 0x010,
    "irq_pending": 0x011,
    "irq_vector": 0x012,
    "cycle": 0xC00,
    "instret": 0xC01,
    "timer": 0xC02,
    "timer_cmp": 0xC03,
}


def mask_bits(value: int, width: int) -> int:
    return value & ((1 << width) - 1)


def check_signed_range(value: int, bits: int, loc: Optional[SourceLocation] = None) -> None:
    min_val = -(1 << (bits - 1))
    max_val = (1 << (bits - 1)) - 1
    if value < min_val or value > max_val:
        msg = f"value {value} out of range for signed {bits}-bit field"
        if loc:
            raise AssemblerError(f"{loc}: {msg}")
        raise AssemblerError(msg)


def check_unsigned_range(value: int, bits: int, loc: Optional[SourceLocation] = None) -> None:
    if value < 0 or value >= (1 << bits):
        msg = f"value {value} out of range for unsigned {bits}-bit field"
        if loc:
            raise AssemblerError(f"{loc}: {msg}")
        raise AssemblerError(msg)


def encode_alu(rd: Register, rsA: Register, rsD: Register, funct: int, swap: bool) -> int:
    value = 0
    value |= (OP_ALU & 0xF) << 44
    value |= (funct & 0xF) << 40
    value |= (rd.rd_bank & 0x1) << 39
    value |= (rd.index & 0x7) << 36
    value |= (rsA.index & 0x7) << 33
    value |= (rsD.index & 0x7) << 30
    value |= (1 if swap else 0) << 29
    return value


def encode_alui_a(rd: Register, rsA: Register, funct: int, imm: int, loc: Optional[SourceLocation] = None) -> int:
    check_signed_range(imm, 27, loc)
    value = 0
    value |= (OP_ALUI_A & 0xF) << 44
    value |= (rd.rd_bank & 0x1) << 39
    value |= (rd.index & 0x7) << 36
    value |= (rsA.index & 0x7) << 33
    value |= (funct & 0x3F) << 27
    value |= mask_bits(imm, 27)
    return value


def encode_alui_d(rd: Register, rsD: Register, funct: int, imm: int, loc: Optional[SourceLocation] = None) -> int:
    check_signed_range(imm, 27, loc)
    value = 0
    value |= (OP_ALUI_D & 0xF) << 44
    value |= (rd.rd_bank & 0x1) << 39
    value |= (rd.index & 0x7) << 36
    value |= (rsD.index & 0x7) << 33
    value |= (funct & 0x3F) << 27
    value |= mask_bits(imm, 27)
    return value


def encode_ld(postinc: bool, dest: Register, base: Register, disp: int, loc: Optional[SourceLocation] = None) -> int:
    if dest.bank != "D":
        raise AssemblerError(f"{loc}: load destination must be D register")
    if base.bank != "A":
        raise AssemblerError(f"{loc}: load base must be A register")
    check_signed_range(disp, 33, loc)
    value = 0
    value |= (OP_LD & 0xF) << 44
    value |= (1 if postinc else 0) << 39
    value |= (dest.index & 0x7) << 36
    value |= (base.index & 0x7) << 33
    value |= mask_bits(disp, 33)
    return value


def encode_st(postinc: bool, src: Register, base: Register, disp: int, loc: Optional[SourceLocation] = None) -> int:
    if src.bank != "D":
        raise AssemblerError(f"{loc}: store source must be D register")
    if base.bank != "A":
        raise AssemblerError(f"{loc}: store base must be A register")
    check_signed_range(disp, 33, loc)
    value = 0
    value |= (OP_ST & 0xF) << 44
    value |= (1 if postinc else 0) << 39
    value |= (src.index & 0x7) << 36
    value |= (base.index & 0x7) << 33
    value |= mask_bits(disp, 33)
    return value


def encode_branch(cond: int, rsA: Register, rsD: Register, offset: int, loc: Optional[SourceLocation] = None) -> int:
    check_signed_range(offset, 31, loc)
    value = 0
    value |= (OP_BR & 0xF) << 44
    value |= (cond & 0x7) << 37
    value |= (rsA.index & 0x7) << 34
    value |= (rsD.index & 0x7) << 31
    value |= mask_bits(offset, 31)
    return value


def encode_jal(rd: Register, offset: int, loc: Optional[SourceLocation] = None) -> int:
    check_signed_range(offset, 36, loc)
    value = 0
    value |= (OP_JAL & 0xF) << 44
    value |= (rd.rd_bank & 0x1) << 39
    value |= (rd.index & 0x7) << 36
    value |= mask_bits(offset, 36)
    return value


def encode_jalr(rd: Register, base: Register, imm: int, loc: Optional[SourceLocation] = None) -> int:
    if base.bank != "A":
        raise AssemblerError(f"{loc}: JALR base must be A register")
    check_signed_range(imm, 33, loc)
    value = 0
    value |= (OP_JALR & 0xF) << 44
    value |= (rd.rd_bank & 0x1) << 39
    value |= (rd.index & 0x7) << 36
    value |= (base.index & 0x7) << 33
    value |= mask_bits(imm, 33)
    return value


def encode_sys(funct: int) -> int:
    value = 0
    value |= (OP_SYS & 0xF) << 44
    value |= (funct & 0xF) << 40
    return value


def encode_csr(funct: int, rd: Register, rs: Register, csr_addr: int, loc: Optional[SourceLocation] = None) -> int:
    if rs.bank != "D":
        raise AssemblerError(f"{loc}: CSR source must be D register")
    check_unsigned_range(csr_addr, 12, loc)
    value = 0
    value |= (OP_CSR & 0xF) << 44
    value |= (funct & 0xF) << 40
    value |= (rd.rd_bank & 0x1) << 39
    value |= (rd.index & 0x7) << 36
    value |= (rs.index & 0x7) << 30
    value |= (csr_addr & 0xFFF) << 12
    return value


@dataclasses.dataclass
class Statement:
    loc: SourceLocation
    base: str
    suffixes: Tuple[str, ...]
    args: List[str]
    labels: List[str]
    raw: str
    is_directive: bool


@dataclasses.dataclass
class EquDefinition:
    name: str
    expr: str
    loc: SourceLocation
    address: int


@dataclasses.dataclass
class InstructionSpec:
    size: int
    encoder: Callable[["Assembler", int, SourceLocation], List[int]]


@dataclasses.dataclass
class Entry:
    kind: str  # 'instruction' or 'word'
    loc: SourceLocation
    address: int
    payload: object


class ExpressionEvaluator:
    def __init__(self, assembler: "Assembler") -> None:
        self.assembler = assembler

    def eval(self, expr: str, current_addr: int, loc: SourceLocation) -> int:
        try:
            node = ast.parse(expr, mode="eval")
        except SyntaxError as exc:
            raise AssemblerError(f"{loc}: invalid expression '{expr}': {exc}") from None
        return self._eval_node(node.body, current_addr, loc)

    def _eval_node(self, node: ast.AST, current_addr: int, loc: SourceLocation) -> int:
        if isinstance(node, ast.Constant):
            if isinstance(node.value, (int, bool)):
                return int(node.value)
            raise AssemblerError(f"{loc}: unsupported constant type {type(node.value).__name__}")
        if isinstance(node, ast.UnaryOp):
            value = self._eval_node(node.operand, current_addr, loc)
            if isinstance(node.op, ast.UAdd):
                return +value
            if isinstance(node.op, ast.USub):
                return -value
            if isinstance(node.op, ast.Invert):
                return ~value
            raise AssemblerError(f"{loc}: unsupported unary operator {ast.dump(node.op)}")
        if isinstance(node, ast.BinOp):
            left = self._eval_node(node.left, current_addr, loc)
            right = self._eval_node(node.right, current_addr, loc)
            op = node.op
            if isinstance(op, ast.Add):
                return left + right
            if isinstance(op, ast.Sub):
                return left - right
            if isinstance(op, ast.Mult):
                return left * right
            if isinstance(op, (ast.Div, ast.FloorDiv)):
                return left // right
            if isinstance(op, ast.Mod):
                return left % right
            if isinstance(op, ast.LShift):
                return left << right
            if isinstance(op, ast.RShift):
                return left >> right
            if isinstance(op, ast.BitOr):
                return left | right
            if isinstance(op, ast.BitXor):
                return left ^ right
            if isinstance(op, ast.BitAnd):
                return left & right
            raise AssemblerError(f"{loc}: unsupported binary operator {ast.dump(op)}")
        if isinstance(node, ast.Name):
            return self.assembler.resolve_symbol(node.id, current_addr, loc)
        if hasattr(ast, "Paren") and isinstance(node, getattr(ast, "Paren")):
            return self._eval_node(node.expression, current_addr, loc)  # type: ignore[attr-defined]
        raise AssemblerError(f"{loc}: unsupported expression element {ast.dump(node)}")


class Assembler:
    def __init__(self) -> None:
        self.labels: Dict[str, int] = {}
        self.equ_defs: Dict[str, EquDefinition] = {}
        self.constants: Dict[str, int] = {}
        self._const_stack: List[str] = []
        self.entries: List[Entry] = []
        self.evaluator = ExpressionEvaluator(self)

    # ------------------------------------------------------------------
    # Public API

    def assemble(self, path: pathlib.Path) -> Dict[int, int]:
        raw_lines = read_source(path)
        macros, stripped = collect_macros(raw_lines)
        expander = MacroExpander(macros)
        expanded = expander.expand(stripped)
        statements, trailing_labels = self.parse_statements(expanded)
        self.pass1(statements, trailing_labels)
        self.resolve_constants()
        return self.pass2()

    def eval_expr(self, expr: str, current_addr: int, loc: SourceLocation) -> int:
        return self.evaluator.eval(expr, current_addr, loc)

    def resolve_symbol(self, name: str, current_addr: int, loc: SourceLocation) -> int:
        if name == "." or name.lower() == "pc":
            return current_addr
        if name in self.labels:
            return self.labels[name]
        if name in self.constants:
            return self.constants[name]
        if name in self.equ_defs:
            return self.evaluate_constant(name, loc)
        raise AssemblerError(f"{loc}: undefined symbol '{name}'")

    def evaluate_constant(self, name: str, loc: SourceLocation) -> int:
        if name in self.constants:
            return self.constants[name]
        if name not in self.equ_defs:
            raise AssemblerError(f"{loc}: undefined symbol '{name}'")
        if name in self._const_stack:
            chain = " -> ".join(self._const_stack + [name])
            raise AssemblerError(f"{loc}: circular .equ dependency ({chain})")
        definition = self.equ_defs[name]
        self._const_stack.append(name)
        value = self.evaluator.eval(definition.expr, definition.address, definition.loc)
        self._const_stack.pop()
        self.constants[name] = value
        return value

    def resolve_constants(self) -> None:
        for name in list(self.equ_defs.keys()):
            if name not in self.constants:
                self.evaluate_constant(name, self.equ_defs[name].loc)

    # ------------------------------------------------------------------
    # Parsing and passes

    def parse_statements(self, lines: Sequence[SourceLine]) -> Tuple[List[Statement], List[str]]:
        statements: List[Statement] = []
        pending_labels: List[str] = []
        for line in lines:
            stripped = strip_comment(line.text).strip()
            if not stripped:
                continue
            labels, remainder = extract_labels(stripped)
            if labels:
                pending_labels.extend(labels)
            if not remainder:
                continue
            parts = remainder.split(None, 1)
            op_token = parts[0]
            arg_str = parts[1] if len(parts) > 1 else ""
            if op_token.startswith('.'):
                raw = op_token[1:]
                segments = [seg.lower() for seg in raw.split('.') if seg]
                if not segments:
                    raise AssemblerError(f"{line.loc}: directive name missing after '.'")
                base = '.' + segments[0]
                suffixes = tuple(segments[1:])
                is_directive = True
            else:
                segments = [seg.lower() for seg in op_token.split('.') if seg]
                base = segments[0]
                suffixes = tuple(segments[1:])
                is_directive = False
            args = split_args(arg_str)
            statements.append(
                Statement(
                    loc=line.loc,
                    base=base,
                    suffixes=suffixes,
                    args=args,
                    labels=pending_labels,
                    raw=remainder,
                    is_directive=is_directive,
                )
            )
            pending_labels = []
        return statements, pending_labels

    def pass1(self, statements: Sequence[Statement], trailing_labels: List[str]) -> None:
        location = 0
        for stmt in statements:
            if stmt.base == ".equ":
                self.handle_equ(stmt, location)
                continue
            for label in stmt.labels:
                self.define_label(label, location, stmt.loc)
            if stmt.is_directive:
                location = self.handle_directive(stmt, location)
            else:
                spec = self.compile_instruction(stmt)
                self.entries.append(Entry("instruction", stmt.loc, location, spec))
                location += spec.size
        for label in trailing_labels:
            self.define_label(label, location, SourceLocation(pathlib.Path("<eof>"), 0))

    def handle_equ(self, stmt: Statement, location: int) -> None:
        name: Optional[str] = None
        if stmt.labels:
            if len(stmt.labels) > 1:
                raise AssemblerError(f"{stmt.loc}: .equ may have at most one label")
            name = stmt.labels[0]
        elif stmt.args:
            name = stmt.args[0]
            stmt.args = stmt.args[1:]
        if not name:
            raise AssemblerError(f"{stmt.loc}: .equ requires a symbol name")
        if not stmt.args:
            raise AssemblerError(f"{stmt.loc}: .equ missing value for '{name}'")
        if name in self.equ_defs or name in self.labels:
            raise AssemblerError(f"{stmt.loc}: symbol '{name}' already defined")
        expr = stmt.args[0]
        self.equ_defs[name] = EquDefinition(name=name, expr=expr, loc=stmt.loc, address=location)

    def handle_directive(self, stmt: Statement, location: int) -> int:
        if stmt.base == ".org":
            if not stmt.args:
                raise AssemblerError(f"{stmt.loc}: .org requires an address")
            new_loc = self.eval_expr(stmt.args[0], location, stmt.loc)
            if new_loc < 0:
                raise AssemblerError(f"{stmt.loc}: .org address must be non-negative")
            return new_loc
        if stmt.base == ".word":
            if not stmt.args:
                raise AssemblerError(f"{stmt.loc}: .word requires at least one value")
            self.entries.append(Entry("word", stmt.loc, location, list(stmt.args)))
            return location + len(stmt.args)
        raise AssemblerError(f"{stmt.loc}: unknown directive '{stmt.base}'")

    def define_label(self, name: str, address: int, loc: SourceLocation) -> None:
        if name in self.labels or name in self.equ_defs:
            raise AssemblerError(f"{loc}: label '{name}' already defined")
        self.labels[name] = address

    def pass2(self) -> Dict[int, int]:
        image: Dict[int, int] = {}
        for entry in self.entries:
            if entry.kind == "instruction":
                spec: InstructionSpec = entry.payload  # type: ignore[assignment]
                words = spec.encoder(self, entry.address, entry.loc)
                if len(words) != spec.size:
                    raise AssemblerError(f"{entry.loc}: encoder produced wrong word count")
                for offset, word in enumerate(words):
                    addr = entry.address + offset
                    if addr in image:
                        raise AssemblerError(f"{entry.loc}: address {addr} already written")
                    image[addr] = mask_bits(word, 48)
            elif entry.kind == "word":
                exprs: List[str] = entry.payload  # type: ignore[assignment]
                for offset, expr in enumerate(exprs):
                    addr = entry.address + offset
                    if addr in image:
                        raise AssemblerError(f"{entry.loc}: address {addr} already written")
                    value = self.eval_expr(expr, addr, entry.loc)
                    check_signed_range(value, 48, entry.loc)
                    image[addr] = mask_bits(value, 48)
            else:
                raise AssemblerError(f"{entry.loc}: unknown entry type '{entry.kind}'")
        return image
    # ------------------------------------------------------------------
    # Instruction compilation helpers

    def compile_instruction(self, stmt: Statement) -> InstructionSpec:
        base = stmt.base
        suffixes = list(stmt.suffixes)
        if base == "copy":
            return self._compile_copy(stmt)
        if base in {"add", "and", "or", "xor", "subtract"}:
            return self._compile_arithmetic(stmt, base, suffixes)
        if base in {"shift_left", "shift_right", "shift_right_arith"}:
            return self._compile_shift(stmt, base, suffixes)
        if base in {"invert", "not"}:
            return self._compile_invert(stmt)
        if base == "load":
            return self._compile_load(stmt, suffixes)
        if base == "store":
            return self._compile_store(stmt, suffixes)
        if base == "branch":
            return self._compile_branch(stmt, suffixes)
        if base == "jump":
            return self._compile_jump(stmt, suffixes)
        if base == "csr":
            return self._compile_csr(stmt, suffixes)
        if base in {"halt", "nop"}:
            funct = 0xF if base == "halt" else 0x0
            return InstructionSpec(1, lambda asm, addr, loc: [encode_sys(funct)])
        if base == "iret":
            return InstructionSpec(1, lambda asm, addr, loc: [encode_sys(0x2)])
        if base == "system":
            if not stmt.args:
                raise AssemblerError(f"{stmt.loc}: system requires a funct value")
            expr = stmt.args[0]
            def encoder(asm: "Assembler", addr: int, loc: SourceLocation) -> List[int]:
                value = asm.eval_expr(expr, addr, loc)
                return [encode_sys(value & 0xF)]
            return InstructionSpec(1, encoder)
        raise AssemblerError(f"{stmt.loc}: unknown instruction '{base}'")

    def _compile_copy(self, stmt: Statement) -> InstructionSpec:
        if len(stmt.args) != 2:
            raise AssemblerError(f"{stmt.loc}: copy expects 'copy dest, src'")
        dest = parse_register(stmt.args[0], loc=stmt.loc)
        src = parse_register(stmt.args[1], loc=stmt.loc)
        def encoder(asm: "Assembler", addr: int, loc: SourceLocation) -> List[int]:
            if src.bank == "A":
                word = encode_alui_a(dest, src, F_ADD, 0, loc)
            else:
                word = encode_alui_d(dest, src, F_ADD, 0, loc)
            return [word]
        return InstructionSpec(1, encoder)

    def _compile_arithmetic(self, stmt: Statement, op: str, suffixes: List[str]) -> InstructionSpec:
        funct_map = {
            "add": F_ADD,
            "and": F_AND,
            "or": F_OR,
            "xor": F_XOR,
            "subtract": F_SUB,
        }
        funct = funct_map[op]
        args = stmt.args
        if len(args) < 2:
            raise AssemblerError(f"{stmt.loc}: {op} missing operands")
        dest = parse_register(args[0], loc=stmt.loc)

        if len(args) == 2:
            src = dest
            imm_expr = args[1]
            if op == "subtract":
                imm_expr = f"-({imm_expr})"
                target_funct = F_ADD
            else:
                target_funct = funct
            def encoder(asm: "Assembler", addr: int, loc: SourceLocation) -> List[int]:
                imm = asm.eval_expr(imm_expr, addr, loc)
                if src.bank == "A":
                    word = encode_alui_a(dest, src, target_funct, imm, loc)
                else:
                    word = encode_alui_d(dest, src, target_funct, imm, loc)
                return [word]
            return InstructionSpec(1, encoder)

        if len(args) != 3:
            raise AssemblerError(f"{stmt.loc}: {op} expects two or three operands")

        third_is_reg = is_register_token(args[2])
        second_is_reg = is_register_token(args[1])

        if third_is_reg and second_is_reg:
            reg_tokens = [args[1], args[2]]
            regs = [parse_register(tok, loc=stmt.loc) for tok in reg_tokens]
            if sum(1 for r in regs if r.bank == "A") != 1 or sum(1 for r in regs if r.bank == "D") != 1:
                raise AssemblerError(f"{stmt.loc}: {op} requires exactly one A and one D register")
            rsA = next(r for r in regs if r.bank == "A")
            rsD = next(r for r in regs if r.bank == "D")
            order_swap = 1 if reg_tokens[0].lower().startswith("d") else 0
            swap = order_swap
            if "swap" in suffixes:
                swap ^= 1
            if op == "subtract" and reg_tokens[0].lower().startswith("a"):
                # default subtract is A - D
                swap ^= 0
            def encoder(asm: "Assembler", addr: int, loc: SourceLocation) -> List[int]:
                word = encode_alu(dest, rsA, rsD, funct if op != "subtract" else F_SUB, bool(swap))
                return [word]
            return InstructionSpec(1, encoder)

        # Immediate form: dest, src, imm
        if not second_is_reg:
            raise AssemblerError(f"{stmt.loc}: {op} expects a register operand before the immediate")
        src = parse_register(args[1], loc=stmt.loc)
        imm_expr = args[2]
        target_funct = funct
        if op == "subtract":
            imm_expr = f"-({imm_expr})"
            target_funct = F_ADD
        def encoder(asm: "Assembler", addr: int, loc: SourceLocation) -> List[int]:
            imm = asm.eval_expr(imm_expr, addr, loc)
            if src.bank == "A":
                word = encode_alui_a(dest, src, target_funct, imm, loc)
            else:
                word = encode_alui_d(dest, src, target_funct, imm, loc)
            return [word]
        return InstructionSpec(1, encoder)
    def _compile_shift(self, stmt: Statement, op: str, suffixes: List[str]) -> InstructionSpec:
        args = stmt.args
        if len(args) < 2:
            raise AssemblerError(f"{stmt.loc}: {op} missing operands")
        dest = parse_register(args[0], loc=stmt.loc)
        funct_map = {
            "shift_left": F_SLL,
            "shift_right": F_SRL,
            "shift_right_arith": F_SRA,
        }
        funct = funct_map[op]

        if len(args) == 2:
            src = dest
            imm_expr = args[1]
            def encoder(asm: "Assembler", addr: int, loc: SourceLocation) -> List[int]:
                imm = asm.eval_expr(imm_expr, addr, loc)
                if src.bank == "A":
                    word = encode_alui_a(dest, src, funct, imm, loc)
                else:
                    word = encode_alui_d(dest, src, funct, imm, loc)
                return [word]
            return InstructionSpec(1, encoder)

        if len(args) != 3:
            raise AssemblerError(f"{stmt.loc}: {op} expects 'dest, reg, operand'")

        if is_register_token(args[2]):
            reg_tokens = [args[1], args[2]]
            regs = [parse_register(tok, loc=stmt.loc) for tok in reg_tokens]
            if sum(1 for r in regs if r.bank == "A") != 1 or sum(1 for r in regs if r.bank == "D") != 1:
                raise AssemblerError(f"{stmt.loc}: {op} requires one A and one D register")
            rsA = next(r for r in regs if r.bank == "A")
            rsD = next(r for r in regs if r.bank == "D")
            order_swap = 1 if reg_tokens[0].lower().startswith("d") else 0
            swap = order_swap
            if "swap" in suffixes:
                swap ^= 1
            def encoder(asm: "Assembler", addr: int, loc: SourceLocation) -> List[int]:
                word = encode_alu(dest, rsA, rsD, funct, bool(swap))
                return [word]
            return InstructionSpec(1, encoder)

        src = parse_register(args[1], loc=stmt.loc)
        imm_expr = args[2]
        def encoder(asm: "Assembler", addr: int, loc: SourceLocation) -> List[int]:
            imm = asm.eval_expr(imm_expr, addr, loc)
            if src.bank == "A":
                word = encode_alui_a(dest, src, funct, imm, loc)
            else:
                word = encode_alui_d(dest, src, funct, imm, loc)
            return [word]
        return InstructionSpec(1, encoder)

    def _compile_invert(self, stmt: Statement) -> InstructionSpec:
        if len(stmt.args) not in (1, 2):
            raise AssemblerError(f"{stmt.loc}: invert expects 'invert dest' or 'invert dest, src'")
        dest = parse_register(stmt.args[0], loc=stmt.loc)
        src = dest if len(stmt.args) == 1 else parse_register(stmt.args[1], loc=stmt.loc)
        def encoder(asm: "Assembler", addr: int, loc: SourceLocation) -> List[int]:
            if src.bank == "A":
                word = encode_alui_a(dest, src, F_NOT, 0, loc)
            else:
                word = encode_alui_d(dest, src, F_NOT, 0, loc)
            return [word]
        return InstructionSpec(1, encoder)

    def _parse_mem_operand(self, token: str, loc: SourceLocation) -> Tuple[str, Register]:
        token = token.strip()
        if "(" in token and token.endswith(")"):
            idx = token.index("(")
            disp = token[:idx].strip() or "0"
            base = token[idx + 1:-1].strip()
            base_reg = parse_register(base, want_bank="A", loc=loc)
            return disp, base_reg
        if "+" in token:
            left, right = token.split("+", 1)
            left = left.strip()
            right = right.strip()
            if is_register_token(left):
                base_reg = parse_register(left, want_bank="A", loc=loc)
                return right, base_reg
            if is_register_token(right):
                base_reg = parse_register(right, want_bank="A", loc=loc)
                return left, base_reg
        if is_register_token(token):
            base_reg = parse_register(token, want_bank="A", loc=loc)
            return "0", base_reg
        raise AssemblerError(f"{loc}: malformed memory operand '{token}'")

    def _compile_load(self, stmt: Statement, suffixes: List[str]) -> InstructionSpec:
        if len(stmt.args) != 2:
            raise AssemblerError(f"{stmt.loc}: load expects 'load dN, offset(aM)'")
        dest = parse_register(stmt.args[0], want_bank="D", loc=stmt.loc)
        disp_expr, base = self._parse_mem_operand(stmt.args[1], stmt.loc)
        postinc = "post" in suffixes
        def encoder(asm: "Assembler", addr: int, loc: SourceLocation) -> List[int]:
            disp = asm.eval_expr(disp_expr, addr, loc)
            word = encode_ld(postinc, dest, base, disp, loc)
            return [word]
        return InstructionSpec(1, encoder)

    def _compile_store(self, stmt: Statement, suffixes: List[str]) -> InstructionSpec:
        if len(stmt.args) != 2:
            raise AssemblerError(f"{stmt.loc}: store expects 'store dN, offset(aM)'")
        src = parse_register(stmt.args[0], want_bank="D", loc=stmt.loc)
        disp_expr, base = self._parse_mem_operand(stmt.args[1], stmt.loc)
        postinc = "post" in suffixes
        def encoder(asm: "Assembler", addr: int, loc: SourceLocation) -> List[int]:
            disp = asm.eval_expr(disp_expr, addr, loc)
            word = encode_st(postinc, src, base, disp, loc)
            return [word]
        return InstructionSpec(1, encoder)
    def _compile_branch(self, stmt: Statement, suffixes: List[str]) -> InstructionSpec:
        if not stmt.args:
            raise AssemblerError(f"{stmt.loc}: branch requires operands")
        condition = None
        cond_candidates = ["eq", "ne", "lt", "le", "gt", "ge", "ltu", "leu", "gtu", "geu", "always", "true"]
        for candidate in cond_candidates:
            if candidate in suffixes:
                condition = candidate
                suffixes.remove(candidate)
                break
        if condition is None and suffixes:
            condition = suffixes.pop(0)
        if condition is None:
            raise AssemblerError(f"{stmt.loc}: branch requires a condition suffix (e.g. branch.eq)")
        unsigned = False
        if "u" in suffixes:
            unsigned = True
            suffixes.remove("u")
        if "s" in suffixes:
            suffixes.remove("s")
        if suffixes:
            raise AssemblerError(f"{stmt.loc}: unsupported branch suffixes {suffixes}")

        args = stmt.args
        target_expr: Optional[str] = None
        reg_args: List[str] = []
        if len(args) == 1:
            target_expr = args[0]
        elif len(args) == 3 and not is_register_token(args[0]):
            target_expr = args[0]
            reg_args = args[1:]
        elif len(args) == 3 and is_register_token(args[0]):
            reg_args = args[:2]
            target_expr = args[2]
        else:
            raise AssemblerError(f"{stmt.loc}: branch expects 'branch.cond target, a?, d?'")

        def branch_offset(asm: "Assembler", expr: str, addr: int, loc: SourceLocation) -> int:
            target = asm.eval_expr(expr, addr, loc)
            return target - (addr + 1)

        if condition in {"always", "true"}:
            rsA = Register("A", 0, "a0")
            rsD = Register("D", 0, "d0")
            if target_expr is None:
                raise AssemblerError(f"{stmt.loc}: branch.{condition} requires a target")
            def encoder(asm: "Assembler", addr: int, loc: SourceLocation) -> List[int]:
                offset = branch_offset(asm, target_expr, addr, loc)
                return [encode_branch(C_ALWAYS, rsA, rsD, offset, loc)]
            return InstructionSpec(1, encoder)

        if len(reg_args) != 2:
            raise AssemblerError(f"{stmt.loc}: branch.{condition} requires registers")
        rsA = parse_register(reg_args[0], want_bank="A", loc=stmt.loc)
        rsD = parse_register(reg_args[1], want_bank="D", loc=stmt.loc)
        if target_expr is None:
            raise AssemblerError(f"{stmt.loc}: branch.{condition} requires a target expression")

        cond_map = {
            "eq": C_BEQ,
            "ne": C_BNE,
            "lt": C_BLT,
            "ltu": C_BLTU,
            "ge": C_BGE,
            "geu": C_BGEU,
        }

        if condition in cond_map:
            code = cond_map[condition if not unsigned else condition.replace("t", "tu")]
            def encoder(asm: "Assembler", addr: int, loc: SourceLocation) -> List[int]:
                offset = branch_offset(asm, target_expr, addr, loc)
                return [encode_branch(code, rsA, rsD, offset, loc)]
            return InstructionSpec(1, encoder)

        if condition in {"gt", "gtu"}:
            lt_code = C_BLTU if condition.endswith("u") or unsigned else C_BLT
            def encoder(asm: "Assembler", addr: int, loc: SourceLocation) -> List[int]:
                skip = encode_branch(lt_code, rsA, rsD, 1, loc)
                target_offset = branch_offset(asm, target_expr, addr + 1, loc)
                take = encode_branch(C_BNE, rsA, rsD, target_offset, loc)
                return [skip, take]
            return InstructionSpec(2, encoder)

        if condition in {"le", "leu"}:
            lt_code = C_BLTU if condition.endswith("u") or unsigned else C_BLT
            def encoder(asm: "Assembler", addr: int, loc: SourceLocation) -> List[int]:
                first = encode_branch(lt_code, rsA, rsD, branch_offset(asm, target_expr, addr, loc), loc)
                second = encode_branch(C_BEQ, rsA, rsD, branch_offset(asm, target_expr, addr + 1, loc), loc)
                return [first, second]
            return InstructionSpec(2, encoder)

        if condition == "ge":
            def encoder(asm: "Assembler", addr: int, loc: SourceLocation) -> List[int]:
                offset = branch_offset(asm, target_expr, addr, loc)
                return [encode_branch(C_BGE, rsA, rsD, offset, loc)]
            return InstructionSpec(1, encoder)

        if condition == "geu" or (condition == "ge" and unsigned):
            def encoder(asm: "Assembler", addr: int, loc: SourceLocation) -> List[int]:
                offset = branch_offset(asm, target_expr, addr, loc)
                return [encode_branch(C_BGEU, rsA, rsD, offset, loc)]
            return InstructionSpec(1, encoder)

        raise AssemblerError(f"{stmt.loc}: unsupported branch condition '{condition}'")

    def _compile_jump(self, stmt: Statement, suffixes: List[str]) -> InstructionSpec:
        if suffixes and suffixes[0] in {"ind", "r", "reg"}:
            return self._compile_jump_indirect(stmt, suffixes[1:])
        args = stmt.args
        if not args:
            raise AssemblerError(f"{stmt.loc}: jump requires a target")
        if len(args) == 1:
            rd = Register("A", 0, "a0")
            target_expr = args[0]
        elif len(args) == 2 and is_register_token(args[0]):
            rd = parse_register(args[0], loc=stmt.loc)
            target_expr = args[1]
        else:
            raise AssemblerError(f"{stmt.loc}: jump expects 'jump target' or 'jump rd, target'")

        def encoder(asm: "Assembler", addr: int, loc: SourceLocation) -> List[int]:
            target = asm.eval_expr(target_expr, addr, loc)
            offset = target - (addr + 1)
            return [encode_jal(rd, offset, loc)]

        return InstructionSpec(1, encoder)

    def _compile_jump_indirect(self, stmt: Statement, suffixes: List[str]) -> InstructionSpec:
        args = stmt.args
        if not args:
            raise AssemblerError(f"{stmt.loc}: jump.ind requires operands")
        rd = Register("A", 0, "a0")
        base: Optional[Register] = None
        imm_expr = "0"
        tokens = list(args)

        if is_register_token(tokens[0]):
            reg0 = parse_register(tokens[0], loc=stmt.loc)
            if reg0.bank == "A" and (len(tokens) == 1 or not is_register_token(tokens[1])):
                base = reg0
                tokens = tokens[1:]
            else:
                rd = reg0
                tokens = tokens[1:]

        if base is None:
            if not tokens:
                raise AssemblerError(f"{stmt.loc}: jump.ind missing base register")
            base = parse_register(tokens[0], want_bank="A", loc=stmt.loc)
            tokens = tokens[1:]

        if tokens:
            imm_expr = tokens[0]

        def encoder(asm: "Assembler", addr: int, loc: SourceLocation) -> List[int]:
            imm = asm.eval_expr(imm_expr, addr, loc)
            return [encode_jalr(rd, base, imm, loc)]

        return InstructionSpec(1, encoder)

    def _compile_csr(self, stmt: Statement, suffixes: List[str]) -> InstructionSpec:
        if not suffixes:
            raise AssemblerError(f"{stmt.loc}: csr requires an operation suffix")
        if len(suffixes) > 1:
            raise AssemblerError(f"{stmt.loc}: csr accepts a single suffix, got {suffixes}")
        op = suffixes[0]
        alias = {
            "rw": CSR_F_RW,
            "write": CSR_F_RW,
            "rs": CSR_F_RS,
            "set": CSR_F_RS,
            "rc": CSR_F_RC,
            "clear": CSR_F_RC,
            "r": CSR_F_R,
            "read": CSR_F_R,
        }
        if op not in alias:
            raise AssemblerError(f"{stmt.loc}: unknown csr operation '{op}'")
        funct = alias[op]
        args = stmt.args
        discard_dest = Register("A", 0, "A0")
        zero_src = Register("D", 0, "D0")

        if funct == CSR_F_R:
            if len(args) != 2:
                raise AssemblerError(f"{stmt.loc}: csr.{op} expects 'csr.{op} dest, csr'")
            dest = parse_register(args[0], loc=stmt.loc)
            csr_token = args[1]
            src_reg = zero_src
        else:
            if len(args) == 3:
                dest = parse_register(args[0], loc=stmt.loc)
                csr_token = args[1]
                src_reg = parse_register(args[2], want_bank="D", loc=stmt.loc)
            elif len(args) == 2:
                dest = discard_dest
                csr_token = args[0]
                src_reg = parse_register(args[1], want_bank="D", loc=stmt.loc)
            else:
                raise AssemblerError(f"{stmt.loc}: csr.{op} expects 'csr.{op} dest, csr, src' or 'csr.{op} csr, src'")

        csr_spec = csr_token

        def encoder(asm: "Assembler", addr: int, loc: SourceLocation) -> List[int]:
            name = csr_spec.lower()
            if name in CSR_NAME_TO_ADDR:
                csr_addr = CSR_NAME_TO_ADDR[name]
            else:
                value = asm.eval_expr(csr_spec, addr, loc)
                check_unsigned_range(value, 12, loc)
                csr_addr = value & 0xFFF
            return [encode_csr(funct, dest, src_reg, csr_addr, loc)]

        return InstructionSpec(1, encoder)
# -----------------------------------------------------------------------------
# Output helpers


def format_memh(image: Dict[int, int]) -> str:
    if not image:
        return ""
    lines: List[str] = []
    pointer: Optional[int] = None
    for addr in sorted(image):
        if pointer is None or addr != pointer:
            lines.append(f"@{addr:X}")
            pointer = addr
        lines.append(f"{image[addr]:012X}")
        pointer = addr + 1
    return "\n".join(lines) + "\n"


def format_binary(image: Dict[int, int]) -> bytes:
    if not image:
        return b""
    max_addr = max(image)
    data = bytearray((max_addr + 1) * 6)
    for addr, value in image.items():
        for byte in range(6):
            data[addr * 6 + 5 - byte] = (value >> (8 * byte)) & 0xFF
    return bytes(data)


# -----------------------------------------------------------------------------
# Command-line interface


def parse_args(argv: Optional[Sequence[str]] = None) -> argparse.Namespace:
    parser = argparse.ArgumentParser(description="Assemble AD48 assembly sources")
    parser.add_argument("input", type=pathlib.Path, help="Input assembly file")
    parser.add_argument("-o", "--output", type=pathlib.Path, help="Output file (defaults to stdout)")
    parser.add_argument("--format", choices=("memh", "hex", "bin"), default="memh",
                        help="Output format: memh (default), contiguous hex, or raw binary")
    parser.add_argument("--base-address", type=int, default=None,
                        help="Base address for contiguous hex output")
    return parser.parse_args(argv)


def main(argv: Optional[Sequence[str]] = None) -> int:
    args = parse_args(argv)
    assembler = Assembler()
    try:
        image = assembler.assemble(args.input)
    except AssemblerError as exc:
        print(f"error: {exc}", file=sys.stderr)
        return 1

    if args.format == "bin":
        data = format_binary(image)
        if args.output:
            args.output.write_bytes(data)
        else:
            sys.stdout.buffer.write(data)
        return 0

    if args.format == "memh":
        text = format_memh(image)
    else:
        if not image:
            text = ""
        else:
            start = args.base_address if args.base_address is not None else min(image)
            end = max(image)
            lines = []
            for addr in range(start, end + 1):
                lines.append(f"{image.get(addr, 0):012X}")
            text = "\n".join(lines) + "\n"
    if args.output:
        args.output.write_text(text, encoding="utf-8")
    else:
        sys.stdout.write(text)
    return 0


if __name__ == "__main__":
    sys.exit(main())
