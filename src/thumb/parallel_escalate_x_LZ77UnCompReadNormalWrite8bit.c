asm void LZ77UnCompReadNormalWrite8bit(const void *source, void *destination)
{
    swi 0x11
    bx lr
}
