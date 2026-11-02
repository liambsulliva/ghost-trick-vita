asm void func_0200093c(void)
{
    add r12, r1, r2
@1:
    cmp r1, r12
    stmltia r1!, {r0}
    blt @1
    bx lr
}
