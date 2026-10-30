asm void func_02072ec4(register unsigned int data, register void *destp, register unsigned int size)
{
    add r12, r1, r2
@30:
    cmp r1, r12
    stmltia r1!, {r0}
    blt @30
    bx lr
}
