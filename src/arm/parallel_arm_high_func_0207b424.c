asm unsigned int func_0207b424(register unsigned int value)
{
    cmp r0, #0
    bxeq lr
    mov r3, #158
    bmi @norm
    clz r12, r0
    movs r0, r0, lsl r12
    sub r3, r3, r12
@norm:
    ands r2, r0, #255
    add r0, r0, r0
    mov r0, r0, lsr #9
    orr r0, r0, r3, lsl #23
    bxeq lr
    tst r2, #128
    bxeq lr
    ands r1, r2, #127
    andeqs r1, r0, #1
    addne r0, r0, #1
    bx lr
}
