asm void func_0206e5c8(register int *pDst, register int sinVal, register int cosVal)
{
    str r2, [r0, #0]
    str r1, [r0, #4]
    neg r1, r1
    str r1, [r0, #8]
    str r2, [r0, #12]
    bx lr
}
