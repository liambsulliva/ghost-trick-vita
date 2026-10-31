asm void CpuSet(void)
{
    swi 0xb
    bx lr
}
