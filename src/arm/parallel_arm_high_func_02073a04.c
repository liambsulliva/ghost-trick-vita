asm unsigned int func_02073a04(void)
{
    ldr r3, =0x02ffffb0
    ldr r1, [r3]
    clz r2, r1
    cmp r2, #32
    movne r0, #64
    bne @tail
    add r3, r3, #4
    ldr r1, [r3]
    clz r2, r1
    cmp r2, #32
    ldr r0, =0xfffffffd
    bxeq lr
    mov r0, #96
@tail:
    add r0, r0, r2
    mov r1, #0x80000000
    mov r1, r1, lsr r2
    ldr r2, [r3]
    bic r2, r2, r1
    str r2, [r3]
    bx lr
}
