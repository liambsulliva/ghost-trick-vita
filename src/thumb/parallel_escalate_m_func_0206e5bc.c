asm void func_0206e5bc(register int *pDst, register int x, register int y)
{
    stmia r0!, {r1}
    mov r1, #0
    str r2, [r0, #8]
    mov r2, #0
    stmia r0!, {r1, r2}
    bx lr
}
