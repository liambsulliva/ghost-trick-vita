asm void func_02072f54(register const void *srcp, register void *destp, register unsigned int size)
{
    stmfd sp!, {r4-r10}
    add r10, r1, r2
    mov r12, r2, lsr #5
    add r12, r1, r12, lsl #5
@10:
    cmp r1, r12
    ldmltia r0!, {r2-r9}
    stmltia r1!, {r2-r9}
    blt @10
@20:
    cmp r1, r10
    ldmltia r0!, {r2}
    stmltia r1!, {r2}
    blt @20
    ldmfd sp!, {r4-r10}
    bx lr
}
