asm unsigned int func_0207bab8(register unsigned int value)
{
    tst r0, #0x80000000
    bne @1
    mov r1, #158
    subs r1, r1, r0, lsr #23
    blt @2
    mov r2, r0, lsl #8
    orr r0, r2, #0x80000000
    mov r0, r0, lsr r1
    bx lr
@1:
    mov r2, #0xff000000
    cmp r2, r0, lsl #1
    movcs r0, #0
    mvncc r0, #0
    bx lr
@2:
    mvn r0, #0
    bx lr
}
