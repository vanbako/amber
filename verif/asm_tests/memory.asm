; AD48 load/store testset covering post-increment, negative offsets, and A0 suppression.
; Scratch registers: a7 (A bank) and d7 (D bank).

.include "macros.inc"

start:
    copy d0, a0

    ; basic store/load
    LOAD_IMM_A a1, 10
    LOAD_IMM_D d1, 0x111
    store d1, 0(a1)
    load d2, 0(a1)
    ASSERT_D_EQ d2, 0x111

    ; post-increment on store updates base
    LOAD_IMM_A a2, 20
    LOAD_IMM_D d2, 0x222
    store.post d2, 0(a2)
    ASSERT_A_EQ a2, 20
    load d3, 0(a2)
    ASSERT_D_EQ d3, 0x222

    ; non-zero displacement steers post-increment target
    LOAD_IMM_D d3, 0x333
    store.post d3, 1(a2)
    ASSERT_A_EQ a2, 21
    load d4, 0(a2)
    ASSERT_D_EQ d4, 0x333

    ; load.post increments base after fetch
    LOAD_IMM_A a3, 30
    LOAD_IMM_D d4, 0x444
    store d4, 0(a3)
    LOAD_IMM_D d5, 0x555
    store d5, 1(a3)
    load.post d6, 0(a3)
    ASSERT_D_EQ d6, 0x444
    ASSERT_A_EQ a3, 30
    ; second access uses offset to advance base
    load.post d6, 1(a3)
    ASSERT_D_EQ d6, 0x555
    ASSERT_A_EQ a3, 31

    ; negative displacement addressing
    LOAD_IMM_A a4, 50
    LOAD_IMM_D d6, 0x666
    store d6, -1(a4)
    load d6, -1(a4)
    ASSERT_D_EQ d6, 0x666

    ; negative results preserved through loads
    LOAD_IMM_D d6, -12345
    store d6, 0(a4)
    load d6, 0(a4)
    ASSERT_D_EQ d6, -12345

    ; base A0 ignores post-increment
    LOAD_IMM_D d1, 0x777
    store d1, 5(a0)
    load.post d2, 5(a0)
    ASSERT_D_EQ d2, 0x777
    ASSERT_A_ZERO a0

    LOAD_IMM_D d3, 0x888
    store.post d3, 6(a0)
    ASSERT_A_ZERO a0
    load d4, 6(a0)
    ASSERT_D_EQ d4, 0x888

    ; large positive offset within DMEM range
    LOAD_IMM_A a5, 100
    LOAD_IMM_D d5, 0x999
    store d5, -2(a5)
    load d6, -2(a5)
    ASSERT_D_EQ d6, 0x999

    halt

fail:
    branch.always fail
