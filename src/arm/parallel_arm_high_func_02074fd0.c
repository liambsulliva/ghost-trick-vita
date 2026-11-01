asm void func_02074fd0(register unsigned int mask)
{
    mrc p15, 0, r1, c3, c0, 0
    orr r1, r1, r0
    mcr p15, 0, r1, c3, c0, 0
    bx lr
}
