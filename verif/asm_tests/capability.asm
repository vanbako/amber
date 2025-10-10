; AD48 capability regression: exercise CSR programming, permission checks,
; bounds faults, and valid-tag handling for A-bank sidecars.

.include "macros.inc"

.equ DATA_BASE,      0x040
.equ RESUME_SLOT,    0x180
.equ STACK_BASE,     0x1C0

start:
    jump.always main

fail:
    branch.always fail

hang:
    branch.always hang

.org 64
handler:
    csr.read d4, cause
    csr.read d5, epc
    csr.read d6, lr

    LOAD_IMM_A a5, RESUME_SLOT
    load d0, 0(a5)
    csr.write lr, d0
    iret

.org 128
main:
    copy d0, a0

    ; Point the supervisor stack used by the trap handler.
    LOAD_IMM_D d0, STACK_BASE
    csr.write ssp, d0

    ; Seed resume slot with a safe default (should never be used).
    LOAD_IMM_A a5, RESUME_SLOT
    LOAD_IMM_D d0, hang
    store d0, 0(a5)

    ; Populate DMEM words that the capability window will expose.
    LOAD_IMM_A a1, DATA_BASE
    LOAD_IMM_D d1, 0x111
    store d1, 0(a1)
    LOAD_IMM_D d2, 0x222
    store d2, 1(a1)

    ; Constrain A1 to a single-word RW window.
    LOAD_IMM_D d3, DATA_BASE
    csr.write cap_base1, d3
    csr.write cap_limit1, d3
    LOAD_IMM_D d3, 0xB        ; valid=1, perms=RW
    csr.write cap_perm1, d3

    ; In-bounds load/store pass under capability enforcement.
    load d0, 0(a1)
    ASSERT_D_EQ d0, 0x111

    LOAD_IMM_D d0, 0x333
    store d0, 0(a1)
    load d0, 0(a1)
    ASSERT_D_EQ d0, 0x333

    ; Capability fault on post-increment store beyond the limit.
    LOAD_IMM_D d3, resume_after_store_fault
    LOAD_IMM_A a5, RESUME_SLOT
    store d3, 0(a5)
    LOAD_IMM_D d0, 0x444
store_fault:
    store.post d0, 1(a1)
resume_after_store_fault:
    ASSERT_D_EQ d4, 9              ; CAUSE_CAPABILITY_STORE
    LOAD_IMM_A a2, DATA_BASE + 1
    load d1, 0(a2)
    ASSERT_D_EQ d1, 0x222          ; store suppressed
    ASSERT_A_EQ a1, DATA_BASE      ; post-increment blocked

    ; Capability fault when addressing below the base.
    LOAD_IMM_D d3, resume_after_low_fault
    LOAD_IMM_A a5, RESUME_SLOT
    store d3, 0(a5)
load_low_fault:
    load d1, -1(a1)
resume_after_low_fault:
    ASSERT_D_EQ d4, 8              ; CAUSE_CAPABILITY_LOAD

    ; Disable the valid bit: even in-range loads must fault.
    LOAD_IMM_D d3, 0x3             ; perms=RW, valid=0
    csr.write cap_perm1, d3
    LOAD_IMM_D d2, resume_after_invalid_fault
    LOAD_IMM_A a5, RESUME_SLOT
    store d2, 0(a5)
invalid_fault:
    load d1, 0(a1)
resume_after_invalid_fault:
    ASSERT_D_EQ d4, 8

    ; Restore the capability to a full-range, all-permission sidecar.
    LOAD_IMM_D d3, 0
    csr.write cap_base1, d3
    LOAD_IMM_D d3, -1
    csr.write cap_limit1, d3
    LOAD_IMM_D d3, 0xF             ; valid=1, perms=RWX
    csr.write cap_perm1, d3

    halt
