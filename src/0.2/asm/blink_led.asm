# Amber 0.2 LED blink demo for the Tang Mega 138K Dock
# Generates roughly 1 Hz toggling on LED[0] by busy-waiting.

start:
    li r1, 0                 # DDC base
    li r2, 0x1000            # DDC bound (4 KiB)
    capmov c4, r1            # c4 = default data capability covering DMEM

    li r3, 1                 # LED on value (low 64b)
    li r4, 0                 # LED on value (high 64b)
    capmov c5, r3            # c5 = 0x0000...0001

    li r5, 0                 # LED off value (low 64b)
    li r6, 0                 # LED off value (high 64b)
    capmov c6, r5            # c6 = 0x0

    li r7, 1                 # constant 1 for loop decrements

blink_loop:
    st128 [c4 + 255], c5     # turn LED on

    li r1, 150               # outer loop count
outer_on:
    li r2, 180               # middle loop count
mid_on:
    li r3, 1000              # inner loop count
inner_on:
    sub r3, r7
    bne r3, r0, inner_on
    sub r2, r7
    bne r2, r0, mid_on
    sub r1, r7
    bne r1, r0, outer_on

    st128 [c4 + 255], c6     # turn LED off

    li r1, 150               # outer loop count
outer_off:
    li r2, 180               # middle loop count
mid_off:
    li r3, 1000              # inner loop count
inner_off:
    sub r3, r7
    bne r3, r0, inner_off
    sub r2, r7
    bne r2, r0, mid_off
    sub r1, r7
    bne r1, r0, outer_off

    br blink_loop
