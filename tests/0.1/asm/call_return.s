# Amber48 jump and call/return coverage stub

        jump start

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

start:
        jump_sub func1
        nop
        load r1, 0(r0)
        nop
        add_imm r2, r0, 55
        nop
        branch_ne r1, r2, fail

        jump_sub func2
        nop
        load r3, 8(r0)
        nop
        add_imm r4, r0, 99
        nop
        branch_ne r3, r4, fail

        jump success

func1:
        add_imm r1, r0, 55
        nop
        store r1, 0(r0)
        nop
        return

func2:
        add_imm r3, r0, 99
        nop
        store r3, 8(r0)
        nop
        add r7, lr, r0
        nop
        return r7

success:
        store r1, 16(r0)
        nop
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
