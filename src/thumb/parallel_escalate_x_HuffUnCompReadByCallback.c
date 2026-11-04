asm int HuffUnCompReadByCallback(const void *source, void *destination, unsigned int size, const void *stream)
{
    swi 0x13
    bx lr
}
