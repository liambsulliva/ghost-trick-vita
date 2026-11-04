asm void BitUnPack(const void *source, void *destination, const void *params)
{
    swi 0x10
    bx lr
}
