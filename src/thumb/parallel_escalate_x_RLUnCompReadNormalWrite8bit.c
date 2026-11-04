asm void RLUnCompReadNormalWrite8bit(const void *source, void *destination)
{
    swi 0x14
    bx lr
}
