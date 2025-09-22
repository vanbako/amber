# Amber48 smoke test stub

        add_imm r1, r0, 0          # sum accumulator
        add_imm r2, r0, 0          # loop counter
        add_imm r3, r0, 16         # loop limit
        add_imm r4, r0, 10         # per-iteration increment
        add_imm r5, r0, 160        # expected sum after loop
        nop

loop:
        add r1, r1, r4
        nop
        add_imm r2, r2, 1
        nop
        branch_ne r2, r3, loop

        store r1, 0(r0)            # write result to data word 0
        load r6, 0(r0)
        nop
        branch_ne r6, r5, fail

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
