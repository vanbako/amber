# Amber48 out-of-bounds data memory trap stub

        add_imm r1, r0, 1
        store r1, 4096(r0)
        nop

hold:
        branch_always hold
