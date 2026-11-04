asm int LZ77UnCompReadByCallbackWrite16bit(const void *source, void *destination, unsigned int size, const void *stream)
{
    swi 0x12
    bx lr
}
