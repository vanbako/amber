# Amber48 smoke test + periodic UART output

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

        # Indicate PASS once, then enter periodic output loop
        add_imm r8, r0, 'P'
        nop
        store r8, 2032(r0)         # UART MMIO (DMEM index DEPTH-2)
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
        add_imm r8, r0, 10         # '\n'
        nop
        store r8, 2032(r0)
        nop

periodic:
        # Print "TICK\n" every ~5 seconds (approximate busy-wait)
        add_imm r8, r0, 'T'
        nop
        store r8, 2032(r0)
        nop
        add_imm r8, r0, 'I'
        nop
        store r8, 2032(r0)
        nop
        add_imm r8, r0, 'C'
        nop
        store r8, 2032(r0)
        nop
        add_imm r8, r0, 'K'
        nop
        store r8, 2032(r0)
        nop
        add_imm r8, r0, 10         # '\n'
        nop
        store r8, 2032(r0)
        nop

        # Delay loop tuned for ~5 s at 27 MHz
        # Inner loop: r7 counts down from 30000
        # Outer loop: r9 repeats inner loop 2250 times
        add_imm r9, r0, 2250       # outer count

delay_outer:
        add_imm r7, r0, 30000      # inner count

delay_inner:
        sub_imm r7, r7, 1
        nop
        branch_notzero r7, delay_inner
        nop

        sub_imm r9, r9, 1
        nop
        branch_notzero r9, delay_outer
        nop

        branch_always periodic     # repeat forever

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
