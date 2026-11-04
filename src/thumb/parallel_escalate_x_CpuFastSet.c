asm void CpuFastSet(const void *source, void *destination, unsigned int mode)
{
    swi 0xc
    bx lr
}
