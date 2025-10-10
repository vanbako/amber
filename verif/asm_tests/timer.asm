; AD48 timer regression: validates machine timer CSR behaviour, compare re-arming,
; and the dedicated timer interrupt path.

.equ TIMER_IRQ_INDEX, 4
.equ TIMER_IRQ_MASK, (1 << TIMER_IRQ_INDEX)
.equ TIMER_FIRST_CMP, 12
.equ TIMER_INTERVAL, 64
.equ IRQ_VECTOR_BASE, 128
.equ REQUIRED_TICKS, 2

.include "macros.inc"

start:
    copy d0, a0
    copy d1, a0
    copy d2, a0
    copy d3, a0
    copy d4, a0
    copy d5, a0
    copy d6, a0
    copy d7, a0

    LOAD_IMM_D d7, TIMER_IRQ_MASK
    csr.write irq_enable, d0
    csr.write irq_pending, d0
    csr.write timer, d0
    csr.write cause, d0

    LOAD_IMM_D d1, IRQ_VECTOR_BASE
    csr.write irq_vector, d1

    LOAD_IMM_D d2, TIMER_FIRST_CMP
    csr.write timer_cmp, d2

    LOAD_IMM_D d1, 0x43          ; machine mode with MIE=1
    csr.write status, d1

    csr.write irq_enable, d7

    LOAD_IMM_D d4, REQUIRED_TICKS

wait_ticks:
    copy a5, d5
    branch.ne wait_ticks, a5, d4

    ASSERT_D_EQ d5, REQUIRED_TICKS

    csr.read d1, timer
    copy a6, d1
    LOAD_IMM_D d6, TIMER_FIRST_CMP
    branch.lt fail, a6, d6

    csr.read d2, timer_cmp
    copy a6, d2
    branch.ne fail, a6, d3        ; expected compare latched by handler

    csr.read d1, irq_enable
    ASSERT_D_EQ d1, TIMER_IRQ_MASK

    csr.read d2, irq_pending
    ASSERT_D_EQ d2, 0

    csr.read d1, status
    copy a6, d1
    LOAD_IMM_D d6, 0x40
    and d1, a6, d6
    ASSERT_D_EQ d1, 0x40

    csr.read d4, cause
    copy a6, d4
    branch.ge fail, a6, d0        ; high bit must be set (interrupt)
    LOAD_IMM_D d6, 0x3F
    and d4, a6, d6
    ASSERT_D_EQ d4, TIMER_IRQ_INDEX

    halt

fail:
    branch.always fail

.org IRQ_VECTOR_BASE
    iret                ; slot 0 (unused)
    iret                ; slot 1
    iret                ; slot 2
    iret                ; slot 3

timer_handler:
    csr.read d1, timer
    add d5, d5, 1
    LOAD_IMM_D d6, TIMER_INTERVAL
    copy a6, d1
    add d3, a6, d6
    csr.write timer_cmp, d3
    LOAD_IMM_D d7, TIMER_IRQ_MASK
    csr.clear irq_pending, d7
    iret
