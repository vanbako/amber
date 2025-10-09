# Minimal AD48 assembly example using the macro assembler.

.equ LOOP_COUNT, 5

.macro loadimm dst, value
    copy \dst, a0
    add \dst, \dst, \value
.endmacro

start:
    loadimm d1, LOOP_COUNT
    copy a1, a0
    add a1, a1, 1

loop:
    subtract d1, d1, 1
    branch.ne loop, a0, d1

    halt
