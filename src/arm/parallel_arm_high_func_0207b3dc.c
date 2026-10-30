asm unsigned int func_0207b3dc(register unsigned int value)
{
    ands r2, r0, #0x80000000
    rsbmi r0, r0, #0
    cmp r0, #0
    bxeq lr
    clz r3, r0
    movs r0, r0, lsl r3
    rsb r3, r3, #158
    ands r1, r0, #255
    add r0, r0, r0
    orr r0, r2, r0, lsr #9
    orr r0, r0, r3, lsl #23
    bxeq lr
    tst r1, #128
    bxeq lr
    ands r3, r1, #127
    andeqs r3, r0, #1
    addne r0, r0, #1
    bx lr
}
