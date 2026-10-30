asm void func_02072ef0(register const void *srcp, register void *destp, register unsigned int size)
{
    add r12, r0, r2
@10:
    cmp r0, r12
    ldmltia r0!, {r2}
    strlt r2, [r1]
    blt @10
    bx lr
}
