asm void VBlankIntrWait(void)
{
    mov r2, #0
    swi 0x5
    bx lr
}
