asm void func_0206e784(register int *pDst, register int sinVal, register int cosVal)
{
    stmia r0!, {r2}
    mov r3, #0
    stmia r0!, {r1, r3}
    neg r1, r1
    stmia r0!, {r1, r2}
    mov r1, #1
    lsl r1, r1, #12
    str r3, [r0, #0]
    str r3, [r0, #4]
    str r3, [r0, #8]
    str r1, [r0, #12]
    bx lr
}
