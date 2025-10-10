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
