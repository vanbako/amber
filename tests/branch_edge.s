# Amber48 branch-zero coverage stub

        add_imm r1, r0, 0
        nop
        branch_zero r1, zero_ok
        branch_always fail

zero_ok:
        add_imm r2, r0, 7
        nop
        branch_notzero r2, notzero_ok
        branch_always fail

notzero_ok:
        add_imm r2, r0, 0
        nop
        branch_notzero r2, fail

        store r2, 0(r0)
        nop

pass:
        add_imm r8, r0, 'P'
        nop
        store r8, 2032(r0)
        nop
        add_imm r8, r0, 'A'
        nop
        store r8, 2032(r0)
        nop
        add_imm r8, r0, 'S'
        nop
        store r8, 2032(r0)
        nop
        add_imm r8, r0, 'S'
        nop
        store r8, 2032(r0)
        nop
        add_imm r8, r0, 10
        nop
        store r8, 2032(r0)
        nop
        halt

fail:
        add_imm r8, r0, 'F'
        nop
        store r8, 2032(r0)
        nop
        add_imm r8, r0, 'A'
        nop
        store r8, 2032(r0)
        nop
        add_imm r8, r0, 'I'
        nop
        store r8, 2032(r0)
        nop
        add_imm r8, r0, 'L'
        nop
        store r8, 2032(r0)
        nop
        add_imm r8, r0, 10
        nop
        store r8, 2032(r0)
        nop
        halt
