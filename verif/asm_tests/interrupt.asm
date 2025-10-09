; AD48 interrupt regression: exercises IRQ enable/pending CSRs, interrupt entry/return,
; and verifies handler state restoration.

.macro LOAD_IMM_D reg, value
    copy \reg, a0
    add  \reg, \reg, (\value)
.endmacro

.macro ASSERT_D_EQ reg, value
    copy a7, \reg
    LOAD_IMM_D d7, (\value)
    branch.ne fail, a7, d7
.endmacro

start:
    copy d0, a0
    copy d5, a0
    copy d6, a0

    ; Enable IRQ line 0 and seed associated CSRs while interrupts are disabled.
    LOAD_IMM_D d2, 1
    csr.write irq_enable, d2
    csr.write irq_pending, d2
    csr.write cause, d0

    ; Program the IRQ vector base (default 128) explicitly.
    LOAD_IMM_D d3, 128
    csr.write irq_vector, d3

    ; Raising MIE with a pending bit latched should trigger an interrupt.
    LOAD_IMM_D d1, 0x43
    csr.write status, d1

after_interrupt:
    ; Resumes after the handler returns.
    LOAD_IMM_D d6, 1

    ASSERT_D_EQ d5, 1               ; handler increments D5 exactly once

    ; Cause high bit marks interrupt, low bits encode IRQ index (0).
    csr.read d1, cause
    copy a1, d1
    branch.ge fail, a1, d0          ; signed cause must be negative (bit 47 set)
    LOAD_IMM_D d4, 0x3F
    and d1, a1, d4
    ASSERT_D_EQ d1, 0

    ; EPC should point to the instruction after the CSR.SET above.
    csr.read d2, epc
    ASSERT_D_EQ d2, after_interrupt

    ; Pending bit cleared by handler; enable mask preserved; MIE restored.
    csr.read d3, irq_pending
    ASSERT_D_EQ d3, 0

    csr.read d4, irq_enable
    ASSERT_D_EQ d4, 1

    csr.read d5, status
    LOAD_IMM_D d6, 0x40
    copy a1, d5
    and d5, a1, d6
    ASSERT_D_EQ d5, 0x40

    halt

fail:
    branch.always fail

.org 128
handler:
    add d5, d5, 1
    csr.clear irq_pending, d2
    iret
