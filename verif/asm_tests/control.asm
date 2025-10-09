; AD48 control-flow testset: branches, JAL/JALR, and system instructions.
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

start:
    copy d0, a0

    ; branch.eq / branch.ne
    LOAD_IMM_A a1, 42
    LOAD_IMM_D d1, 42
    branch.eq br_eq_taken, a1, d1
    jump fail
br_eq_taken:
    LOAD_IMM_D d1, 41
    branch.eq fail, a1, d1

    branch.ne br_ne_taken, a1, d1
    jump fail
br_ne_taken:
    LOAD_IMM_D d1, 42
    branch.ne fail, a1, d1

    ; signed lt / ge
    LOAD_IMM_A a2, -5
    LOAD_IMM_D d2, 3
    branch.lt br_lt_taken, a2, d2
    jump fail
br_lt_taken:
    LOAD_IMM_D d2, -10
    branch.lt fail, a2, d2

    LOAD_IMM_D d2, -5
    branch.ge br_ge_taken, a2, d2
    jump fail
br_ge_taken:
    LOAD_IMM_D d2, 0
    branch.ge fail, a2, d2

    ; unsigned lt / ge
    LOAD_IMM_A a3, 0
    LOAD_IMM_D d3, -1
    branch.ltu br_ltu_taken, a3, d3
    jump fail
br_ltu_taken:
    branch.ltu fail, a3, d0

    LOAD_IMM_A a3, -1
    LOAD_IMM_D d3, 0
    branch.geu br_geu_taken, a3, d3
    jump fail
br_geu_taken:
    LOAD_IMM_A a3, 0
    LOAD_IMM_D d3, 1
    branch.geu fail, a3, d3

    ; branch.gt / branch.gtu
    LOAD_IMM_A a4, 7
    LOAD_IMM_D d4, 3
    branch.gt br_gt_taken, a4, d4
    jump fail
br_gt_taken:
    LOAD_IMM_D d4, 9
    branch.gt fail, a4, d4

    LOAD_IMM_A a5, -1
    LOAD_IMM_D d5, 1
    branch.gtu br_gtu_taken, a5, d5
    jump fail
br_gtu_taken:
    LOAD_IMM_A a5, 1
    branch.gtu fail, a5, d5

    ; branch.le / branch.leu
    LOAD_IMM_A a6, -8
    LOAD_IMM_D d6, -8
    branch.le br_le_taken, a6, d6
    jump fail
br_le_taken:
    LOAD_IMM_D d6, -10
    branch.le fail, a6, d6

    LOAD_IMM_A a6, 1
    LOAD_IMM_D d6, 3
    branch.leu br_leu_taken, a6, d6
    jump fail
br_leu_taken:
    LOAD_IMM_A a6, -1
    LOAD_IMM_D d6, 0
    branch.leu fail, a6, d6

    ; unconditional branches
    branch.always br_always
    jump fail
br_always:
    branch.true br_true
    jump fail
br_true:

    ; backward branch loop
    LOAD_IMM_A a2, 3
    copy d5, a0
loop_counter:
    add d5, d5, 1
    subtract a2, a2, 1
    branch.ne loop_counter, a2, d0
    ASSERT_D_EQ d5, 3

    ; JAL linking to A register
    jump a6, jal_target_a
jal_return_a:
    ASSERT_A_EQ a6, jal_return_a

    ; JAL linking to D register
    jump d6, jal_target_d
jal_return_d:
    ASSERT_D_EQ d6, jal_return_d

    ; JALR forward offset
    LOAD_IMM_A a4, jalr_forward_base
    jump.ind a5, a4, jalr_forward_target - jalr_forward_base
jalr_forward_continue:
    ASSERT_A_EQ a5, jalr_forward_continue

    ; JALR negative offset
    LOAD_IMM_A a4, jalr_negative_base
    jump.ind a5, a4, jalr_negative_target - jalr_negative_base
jalr_negative_continue:
    ASSERT_A_EQ a5, jalr_negative_continue

    ; JALR linking to D register
    LOAD_IMM_A a4, jalr_d_base
    jump.ind d5, a4, jalr_d_target - jalr_d_base
jalr_d_continue:
    ASSERT_D_EQ d5, jalr_d_continue

    halt

jal_target_a:
    ASSERT_A_EQ a6, jal_return_a
    nop
    system 0
    jump.ind a0, a6

jal_target_d:
    ASSERT_D_EQ d6, jal_return_d
    system 0
    copy a6, d6
    jump.ind a0, a6

jalr_forward_base:
    nop

jalr_forward_target:
    ASSERT_A_EQ a5, jalr_forward_continue
    system 0x0
    jump.ind a0, a5

jalr_negative_target:
    ASSERT_A_EQ a5, jalr_negative_continue
    nop
    jump.ind a0, a5

jalr_negative_base:
    system 0

jalr_d_target:
    ASSERT_D_EQ d5, jalr_d_continue
    copy a4, d5
    jump.ind a0, a4

jalr_d_base:
    nop

fail:
    branch.always fail
