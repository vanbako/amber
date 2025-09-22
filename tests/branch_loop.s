# Amber48 branch verification stub

        add_imm r1, r0, 0          # running sum
        add_imm r2, r0, 5          # loop bound
        add_imm r3, r0, 0          # loop index
        add_imm r4, r0, 2          # increment per iteration
        add_imm r5, r0, 10         # expected sum when loop ends
        nop

loop:
        add r1, r1, r4
        nop
        add_imm r3, r3, 1
        nop
        branch_ne r3, r2, loop

        branch_ne r1, r5, fail

        add_imm r6, r0, 0
        branch_ne r6, r0, fail

        add_imm r6, r0, 1
        branch_eq r6, r0, fail

        branch_eq r3, r2, pass_check
        branch_always fail

pass_check:
        store r1, 0(r0)
        load r7, 0(r0)
        nop
        branch_ne r7, r5, fail

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
