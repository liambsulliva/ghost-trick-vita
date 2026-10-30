asm void func_02073a5c(register unsigned int lockID)
{
    ldr r3, =0x02ffffb0
    cmp r0, #96
    addpl r3, r3, #4
    subpl r0, r0, #96
    submi r0, r0, #64
    mov r1, #0x80000000
    mov r1, r1, lsr r0
    ldr r2, [r3]
    orr r2, r2, r1
    str r2, [r3]
    bx lr
}
