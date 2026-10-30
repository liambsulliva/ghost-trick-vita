asm void func_0206e684(register int *pDst)
{
    mov r2, #4096
    str r2, [r0, #32]
    mov r3, #0
    stmia r0!, {r2, r3}
    mov r1, #0
    stmia r0!, {r1, r3}
    stmia r0!, {r2, r3}
    stmia r0!, {r1, r3}
    bx lr
}
