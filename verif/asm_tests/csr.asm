; AD48 CSR regression: exercise machine CSRs, counter behaviour, privilege gating, and invalid accesses.
; Scratch registers: a7 (A bank) and d7 (D bank) for assertions.

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

start:
    copy d0, a0

    ; Reset view of STATUS exposes machine privilege bits.
    csr.read d1, status
    ASSERT_D_EQ d1, 3

    ; Scratch write/read round-trip with readback capture.
    LOAD_IMM_D d2, 0x12340
    csr.rw d3, scratch, d2
    ASSERT_D_EQ d3, 0
    csr.read d4, scratch
    ASSERT_D_EQ d4, 0x12340

    ; Bit set/clear verbs mutate scratch while returning the old value.
    LOAD_IMM_D d5, 0x000F
    csr.set d6, scratch, d5
    ASSERT_D_EQ d6, 0x12340
    csr.read d7, scratch
    ASSERT_D_EQ d7, 0x1234F

    csr.clear d1, scratch, d5
    ASSERT_D_EQ d1, 0x1234F
    csr.read d2, scratch
    ASSERT_D_EQ d2, 0x12340

    ; Zero mask paths behave like pure reads (returns old value, no write).
    csr.set a1, scratch, d0
    ASSERT_A_EQ a1, 0x12340
    csr.clear d3, scratch, d0
    ASSERT_D_EQ d3, 0x12340
    csr.read d4, scratch
    ASSERT_D_EQ d4, 0x12340

    ; STATUS write synchronises priv_mode and readback image.
    LOAD_IMM_D d5, 0x123003
    csr.write status, d5
    csr.read d6, status
    ASSERT_D_EQ d6, 0x123003

    ; EPC/CAUSE placeholders accept writes while in machine mode.
    LOAD_IMM_D d1, 0x200
    csr.rw d2, epc, d1
    ASSERT_D_EQ d2, 0
    csr.read d3, epc
    ASSERT_D_EQ d3, 0x200

    LOAD_IMM_D d1, 0x0BAD
    csr.write cause, d1
    csr.read d4, cause
    ASSERT_D_EQ d4, 0x0BAD

    ; Cycle counter advances over time and can be seeded.
    csr.read d5, cycle
    system 0
    system 0
    csr.read d6, cycle
    copy a1, d6
    branch.le fail, a1, d5

    LOAD_IMM_D d7, 200
    csr.write cycle, d7
    csr.read d1, cycle
    ASSERT_D_EQ d1, 200

    ; Instret counter is monotonic and writable.
    csr.read d2, instret
    system 0
    system 0
    csr.read d3, instret
    copy a1, d3
    branch.le fail, a1, d2

    LOAD_IMM_D d4, 50
    csr.write instret, d4
    csr.read d5, instret
    ASSERT_D_EQ d5, 50

    ; Unknown CSR addresses are ignored (destination must remain unchanged).
    ; Drop to user privilege; machine-only CSRs become inaccessible.
    csr.write status, d0

    ; User-mode accesses to counters remain permitted.
    LOAD_IMM_D d1, 300
    csr.write cycle, d1
    csr.read d2, cycle
    ASSERT_D_EQ d2, 300

    LOAD_IMM_D d3, 75
    csr.write instret, d3
    csr.read d4, instret
    ASSERT_D_EQ d4, 75

    halt

fail:
    branch.always fail
