asm void func_0206e5a4(register int *pDst)
{
    mov r1, #0
    mov r2, #4096
    mov r3, #0
    stmia r0!, {r2, r3}
    stmia r0!, {r1, r2}
    bx lr
}
