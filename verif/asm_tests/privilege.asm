; AD48 privilege regression: ensure STATUS mode bits drive hardware user access controls.

.macro LOAD_IMM_D reg, value
    copy \reg, a0
    add  \reg, \reg, (\value)
.endmacro

.macro ASSERT_D_EQ reg, value
    copy a7, \reg
    LOAD_IMM_D d0, (\value)
    branch.ne fail, a7, d0
.endmacro

start:
    copy d0, a0

    ; Seed supervisor stack pointer used by the trap handler.
    LOAD_IMM_D d0, 0x120
    csr.write ssp, d0

    ; Enable all interrupt enable bits while remaining in machine mode.
    LOAD_IMM_D d1, 0x73
    csr.write status, d1

    ; Drop to supervisor then user mode.
    LOAD_IMM_D d1, 0x71
    csr.write status, d1

    LOAD_IMM_D d1, 0x74
    csr.write status, d1

user_fault:
    LOAD_IMM_D d2, 1
    csr.write scratch, d2     ; user-mode access must fault

resume_user:
    LOAD_IMM_D d3, 1
    ASSERT_D_EQ d3, 1

    ASSERT_D_EQ d4, 0x2B3     ; trapped STATUS snapshot (machine, prev=user, MPIE/UPIE latched)
    ASSERT_D_EQ d5, 2         ; illegal instruction cause
    ASSERT_D_EQ d6, user_fault + 2
    ASSERT_D_EQ d7, user_fault + 2 ; LR mirrors EPC
    ASSERT_D_EQ d2, 0x120     ; SSP visible through CSR access
    ASSERT_D_EQ d1, 0         ; scratch write suppressed

    halt

fail:
    branch.always fail

.org 64
handler:
    csr.read d4, status
    csr.read d5, cause
    csr.read d6, epc
    csr.read d7, lr
    csr.read d2, ssp
    csr.read d1, scratch
    LOAD_IMM_D d0, resume_user
    csr.write lr, d0
    iret
