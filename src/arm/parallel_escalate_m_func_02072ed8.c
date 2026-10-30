asm void func_02072ed8(register const void *srcp, register void *destp, register unsigned int size)
{
    add r12, r1, r2
@20:
    cmp r1, r12
    ldmltia r0!, {r2}
    stmltia r1!, {r2}
    blt @20
    bx lr
}
