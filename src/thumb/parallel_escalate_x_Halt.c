asm void Halt(void)
{
    swi 0x6
    bx lr
}
