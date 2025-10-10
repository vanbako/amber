; Validate supervisor stack pointer (SSP) and link register (LR) behaviour using a machine-mode breakpoint trap.

.macro LOAD_IMM_A reg, value
    copy \reg, a0
    add  \reg, \reg, (\value)
.endmacro

.macro LOAD_IMM_D reg, value
    copy \reg, a0
    add  \reg, \reg, (\value)
.endmacro

.equ stack_base, 0x010

start:
    LOAD_IMM_D d1, stack_base
    csr.write ssp, d1

trap_point:
    system 1                 ; breakpoint -> trap
skip_label:
    branch.always fail       ; should be skipped when LR is rewritten

resume:
    halt

fail:
    branch.always fail

.org 64
handler:
    LOAD_IMM_D d3, 0x1
    csr.write scratch, d3

    LOAD_IMM_A a4, trap_point
    csr.read d4, lr
    branch.ne fail, a4, d4

    LOAD_IMM_D d3, 0x2
    csr.write scratch, d3

    LOAD_IMM_A a4, stack_base
    csr.read d5, ssp
    branch.ne fail, a4, d5

    LOAD_IMM_D d3, 0x3
    csr.write scratch, d3

    LOAD_IMM_D d0, 1
    add ssp, ssp, d0          ; increment supervisor stack pointer

    LOAD_IMM_A a4, stack_base + 1
    csr.read d6, ssp
    branch.ne fail, a4, d6

    LOAD_IMM_D d3, 0x4
    csr.write scratch, d3

    LOAD_IMM_D d0, resume
    csr.write lr, d0
    LOAD_IMM_A a4, resume
    csr.read d7, lr
    branch.ne fail, a4, d7

    LOAD_IMM_D d3, 0xAA
    csr.write scratch, d3

    iret
