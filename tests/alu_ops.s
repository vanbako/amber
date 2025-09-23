# Amber48 ALU coverage stub

        add_imm r1, r0, 5
        add_imm r2, r0, 7
        nop
        add r3, r1, r2             # 5 + 7 = 12
        nop
        add_imm r4, r0, 12
        nop
        branch_ne r3, r4, fail

        sub r5, r4, r1             # 12 - 5 = 7
        nop
        add_imm r6, r0, 7
        nop
        branch_ne r5, r6, fail

        sub_imm r6, r6, 2          # 7 - 2 = 5
        nop
        add_imm r7, r0, 5
        nop
        branch_ne r6, r7, fail

        and r8, r2, r4             # 7 & 12 = 4
        nop
        add_imm r9, r0, 4
        nop
        branch_ne r8, r9, fail

        or r8, r2, r1              # 7 | 5 = 7
        nop
        add_imm r9, r0, 7
        nop
        branch_ne r8, r9, fail

        xor r8, r2, r1             # 7 ^ 5 = 2
        nop
        add_imm r9, r0, 2
        nop
        branch_ne r8, r9, fail

        xor_imm r8, r9, 3          # 2 ^ 3 = 1
        nop
        add_imm r10, r0, 1
        nop
        branch_ne r8, r10, fail

        add_imm r11, r0, 1
        add_imm r12, r0, 3
        nop
        lsl r13, r11, r12          # 1 << 3 = 8
        nop
        add_imm r14, r0, 8
        nop
        branch_ne r13, r14, fail

        lsr r15, r14, r12          # 8 >> 3 = 1
        nop
        branch_ne r15, r11, fail

        upper_imm r6, 0x123456
        nop
        add_imm r6, r6, 0x78
        nop
        upper_imm r7, 0x123456
        nop
        add_imm r7, r7, 0x78
        branch_ne r6, r7, fail

        store r6, 0(r0)
        nop
        load r8, 0(r0)
        nop
        branch_ne r8, r6, fail

        add_imm r9, r0, 0xAA
        store r9, 2040(r0)         # LED MMIO slot
        nop
        load r10, 2040(r0)
        nop
        branch_ne r10, r9, fail

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
