asm void func_0207b2cc(void)
{
    mov r12, #0x200000
    cmn r12, r1, lsl #1
    bhs @4
    cmn r12, r3, lsl #1
    bhs @5
@1:
    orrs r12, r3, r1
    bmi @3
    cmp r1, r3
    cmpeq r0, r2
    movhi r0, #1
    movls r0, #0
    bx lr
@2:
    mov r0, #0
    mrs r12, CPSR
    bic r12, r12, #0x20000000
    msr CPSR_f, r12
    bx lr
@3:
    orr r12, r0, r12, lsl #1
    orrs r12, r12, r2
    moveq r0, #0
    mrs r12, CPSR
    bic r12, r12, #0x20000000
    msr CPSR_f, r12
    bxeq lr
    cmp r3, r1
    cmpeq r2, r0
    movhi r0, #1
    movls r0, #0
    bx lr
@4:
    bne @2
    cmp r0, #0
    bhi @2
    cmn r12, r3, lsl #1
    blo @1
@5:
    bne @2
    cmp r2, #0
    bhi @2
    b @1
}
