asm void func_02072fa8(register void *dstp, register unsigned int data, register unsigned int size)
{
    cmp r2, #0
    bxeq lr
    tst r0, #1
    beq @10
    ldrh r12, [r0, #-1]
    and r12, r12, #255
    orr r3, r12, r1, lsl #8
    strh r3, [r0, #-1]
    add r0, r0, #1
    subs r2, r2, #1
    bxeq lr
@10:
    cmp r2, #2
    bcc @50
    orr r1, r1, r1, lsl #8
    tst r0, #2
    beq @20
    strh r1, [r0], #2
    subs r2, r2, #2
    bxeq lr
@20:
    orr r1, r1, r1, lsl #16
    bics r3, r2, #3
    beq @40
    sub r2, r2, r3
    add r12, r3, r0
@30:
    str r1, [r0], #4
    cmp r0, r12
    bcc @30
@40:
    tst r2, #2
    strneh r1, [r0], #2
@50:
    tst r2, #1
    bxeq lr
    ldrh r3, [r0]
    and r3, r3, #65280
    and r1, r1, #255
    orr r1, r1, r3
    strh r1, [r0]
    bx lr
}
