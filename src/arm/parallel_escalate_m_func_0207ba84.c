asm int func_0207ba84(int value)
{
    bic r1, r0, #0x80000000
    mov r2, #158
    subs r2, r2, r1, lsr #23
    ble @1
    mov r1, r1, lsl #8
    orr r1, r1, #0x80000000
    cmp r0, #0
    mov r0, r1, lsr r2
    rsbmi r0, r0, #0
    bx lr
@1:
    mvn r0, r0, asr #31
    add r0, r0, #0x80000000
    bx lr
}
