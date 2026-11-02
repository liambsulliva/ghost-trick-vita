asm long long func_0207b364(register unsigned int value)
{
    bic r3, r0, #0x80000000
    mov r2, #189
    subs r2, r2, r3, lsr #23
    blt @big
    cmp r2, #63
    bge @zero
    mov r12, r0, asr #31
    mov r0, r0, lsl #8
    orr r0, r0, #0x80000000
    eor r3, r12, r0, lsr #1
    add r3, r3, r12, lsr #31
    cmp r2, #32
    ble @low
    sub r2, r2, #32
    mov r1, r12
    mov r0, r3, asr r2
    rsb r2, r2, #32
    movs r3, r3, lsl r2
    addne r0, r0, r12, lsr #31
    bx lr
@low:
    mov r1, r3, asr r2
    rsb r2, r2, #32
    mov r0, r3, lsl r2
    bx lr
@zero:
    mov r0, #0
    mov r1, #0
    bx lr
@big:
    mvn r0, r0, asr #31
    add r1, r0, #0x80000000
    bx lr
}
