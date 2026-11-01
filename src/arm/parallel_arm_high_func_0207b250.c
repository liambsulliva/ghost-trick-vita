asm double func_0207b250(register int value)
{
    ands r2, r0, #0x80000000
    rsbmi r0, r0, #0
    cmp r0, #0
    mov r1, #0
    bxeq lr
    mov r3, #1024
    add r3, r3, #30
    clz r12, r0
    movs r0, r0, lsl r12
    sub r3, r3, r12
    movs r1, r0
    mov r0, r1, lsl #21
    add r1, r1, r1
    orr r1, r2, r1, lsr #12
    orr r1, r1, r3, lsl #20
    bx lr
}
