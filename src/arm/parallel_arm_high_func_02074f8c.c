asm void func_02074f8c(register unsigned int mask)
{
    mrc p15, 0, r1, c2, c0, 1
    bic r1, r1, r0
    mcr p15, 0, r1, c2, c0, 1
    bx lr
}
