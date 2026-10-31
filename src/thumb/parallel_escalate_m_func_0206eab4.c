asm void func_0206eab4(register int *pDst, register int sinVal, register int cosVal)
{
    stmia r0!, {r2}
    mov r3, #0
    stmia r0!, {r1, r3}
    neg r1, r1
    stmia r0!, {r1, r2, r3}
    mov r1, #0
    mov r2, #0
    mov r3, #1
    lsl r3, r3, #12
    stmia r0!, {r1, r2, r3}
    mov r3, #0
    stmia r0!, {r1, r2, r3}
    bx lr
}
