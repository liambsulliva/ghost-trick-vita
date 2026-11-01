asm double func_0207b290(register unsigned int value)
{
    cmp r0, #0
    mov r1, #0
    bxeq lr
    mov r3, #1024
    add r3, r3, #30
    bmi @norm_done
    clz r12, r0
    movs r0, r0, lsl r12
    sub r3, r3, r12
@norm_done:
    mov r1, r0
    mov r0, r1, lsl #21
    add r1, r1, r1
    mov r1, r1, lsr #12
    orr r1, r1, r3, lsl #20
    bx lr
}
