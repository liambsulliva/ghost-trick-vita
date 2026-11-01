asm void func_020747e4(void)
{
    mov r1, #0
@outer:
    mov r0, #0
@inner:
    orr r2, r1, r0
    mcr p15, 0, r2, c7, c10, 2
    add r0, r0, #32
    cmp r0, #1024
    blt @inner
    add r1, r1, #0x40000000
    cmp r1, #0
    bne @outer
    bx lr
}
