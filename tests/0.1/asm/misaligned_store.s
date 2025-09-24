# Amber48 misaligned store trap stub

        add_imm r1, r0, 1          # value to store
        nop
        store r1, 4(r0)            # BAU alignment is 8 bytes -> expect trap
        nop

hold:
        branch_always hold         # wait here if trap is not raised
