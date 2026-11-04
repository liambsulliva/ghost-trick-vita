asm int IsDebugger(void)
{
    swi 0xf
    bx lr
}
