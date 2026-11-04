asm void SoftReset(void)
{
    swi 0x0
    bx lr
}
