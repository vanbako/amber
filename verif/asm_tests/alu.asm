; AD48 ALU / immediate testset exercising register/immediate forms, swaps, and extremes.
; Scratch registers: a7 (A bank) and d7 (D bank).

.macro LOAD_IMM_A reg, value
    copy \reg, a0
    add  \reg, \reg, (\value)
.endmacro

.macro LOAD_IMM_D reg, value
    copy \reg, a0
    add  \reg, \reg, (\value)
.endmacro

.macro ASSERT_A_EQ reg, value
    LOAD_IMM_D d7, (\value)
    branch.ne fail, \reg, d7
.endmacro

.macro ASSERT_D_EQ reg, value
    copy a7, \reg
    LOAD_IMM_D d7, (\value)
    branch.ne fail, a7, d7
.endmacro

.macro ASSERT_A_ZERO reg
    branch.ne fail, \reg, d0
.endmacro

.macro ASSERT_D_ZERO reg
    copy a7, \reg
    branch.ne fail, a7, d0
.endmacro

start:
    copy d0, a0

    ; copy between banks
    LOAD_IMM_A a1, 12345
    copy d1, a1
    ASSERT_D_EQ d1, 12345

    LOAD_IMM_D d2, -1234
    copy a2, d2
    ASSERT_A_EQ a2, -1234

    copy d3, d2
    ASSERT_D_EQ d3, -1234

    copy a3, a1
    ASSERT_A_EQ a3, 12345

    ; add/sub immediate forms
    LOAD_IMM_D d4, 100
    add d4, d4, 200
    ASSERT_D_EQ d4, 300
    add d4, d4, -600
    ASSERT_D_EQ d4, -300

    LOAD_IMM_A a4, 2000
    add a4, a4, -750
    ASSERT_A_EQ a4, 1250

    ; register add/sub variants with swap path
    LOAD_IMM_A a5, 900
    LOAD_IMM_D d5, 75
    add d6, a5, d5
    ASSERT_D_EQ d6, 975

    add a5, d5, a4
    ASSERT_A_EQ a5, 1325

    LOAD_IMM_A a6, -55
    add.swap d6, a6, d5
    ASSERT_D_EQ d6, 20

    subtract d6, a5, d5
    ASSERT_D_EQ d6, 1250

    subtract a4, a4, d5
    ASSERT_A_EQ a4, 1175

    ; logical ops
    LOAD_IMM_D d6, 0x1234
    and d6, d6, 0x00FF
    ASSERT_D_EQ d6, 0x0034

    LOAD_IMM_D d6, 0x1234
    or d6, d6, 0x00F0
    ASSERT_D_EQ d6, 0x12F4

    LOAD_IMM_D d6, 0xAAAA
    xor d6, d6, 0x0F0F
    ASSERT_D_EQ d6, 0xA5A5

    xor d6, a4, d5
    ASSERT_D_EQ d6, (1175 ^ 75)

    ; unary invert
    LOAD_IMM_D d6, -1
    invert d6
    ASSERT_D_EQ d6, 0

    invert d6, d5
    ASSERT_D_EQ d6, -76

    ; shift immediates and register-controlled shifts
    LOAD_IMM_D d1, 1
    shift_left d1, d1, 5
    ASSERT_D_EQ d1, 32

    LOAD_IMM_D d2, 0x100
    shift_right d2, d2, 4
    ASSERT_D_EQ d2, 0x10

    LOAD_IMM_D d3, -8
    shift_right_arith d3, d3, 2
    ASSERT_D_EQ d3, -2

    LOAD_IMM_A a4, 3
    LOAD_IMM_D d4, 0x20
    shift_left d4, d4, a4
    ASSERT_D_EQ d4, 0x100

    LOAD_IMM_A a4, -1
    shift_right_arith a4, a4, d1
    ASSERT_A_EQ a4, -1

    LOAD_IMM_A a4, 0x11
    LOAD_IMM_D d4, 4
    shift_left.swap a4, a4, d4
    ASSERT_A_EQ a4, 524288

    ; 27-bit immediate extremes
    LOAD_IMM_D d5, 33554431
    add d5, d5, 1
    ASSERT_D_EQ d5, 33554432

    LOAD_IMM_D d6, -33554432
    add d6, d6, -1
    ASSERT_D_EQ d6, -33554433

    LOAD_IMM_A a5, 1
    shift_left a5, a5, 47
    LOAD_IMM_D d7, 1
    shift_left d7, d7, 47
    branch.ne fail, a5, d7

    ; verify writes to a0 are ignored
    add a0, a0, 5
    ASSERT_A_ZERO a0

    halt

fail:
    branch.always fail
