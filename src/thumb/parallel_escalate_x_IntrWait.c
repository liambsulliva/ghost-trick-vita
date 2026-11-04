asm void IntrWait(int clear, int flags)
{
    swi 0x4
    bx lr
}
