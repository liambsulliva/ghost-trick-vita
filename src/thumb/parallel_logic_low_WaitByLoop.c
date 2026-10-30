asm void WaitByLoop(void)
{
    swi 0x3
    bx lr
}
