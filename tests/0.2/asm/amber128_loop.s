# Simple amber128 program: tight loop using conditional branch

start:
  beq r0, r0, start   # always taken; branches to same bundle (imm=0)

