; Validate that JALR targets outside the instruction memory range raise an
; illegal-instruction trap without committing the link register.

.include "macros.inc"

start:
    copy d0, a0
    copy d3, a0                ; link register under test
    csr.write scratch, d0      ; clear scratch indicator

    LOAD_IMM_A a2, 2048        ; base far beyond any loaded instruction

trap_point:
    jump.ind d3, a2, 0         ; should raise ILLEGAL and enter handler
    jump fail_no_trap

resume_after_trap:
    csr.read d1, scratch       ; handler records trap entry
    copy a1, d1
    LOAD_IMM_D d2, 1
    branch.ne fail, a1, d2
    ASSERT_D_EQ d3, 0          ; link register untouched on illegal target

    csr.read d1, cause
    ASSERT_D_EQ d1, 2          ; CAUSE_ILLEGAL_INSTR

    ASSERT_D_EQ d4, trap_point ; Handler captured faulting EPC

    csr.read d2, epc
    copy a1, d2
    LOAD_IMM_D d5, resume_after_trap
    branch.ne fail, a1, d5

    csr.read d6, lr
    ASSERT_D_EQ d6, resume_after_trap

    halt

fail_no_trap:
    branch.always fail_no_trap

fail:
    branch.always fail

.org 64
handler:
    csr.read d4, epc
    LOAD_IMM_D d5, 1
    csr.write scratch, d5
    LOAD_IMM_D d6, resume_after_trap
    csr.write epc, d6
    iret
