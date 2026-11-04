asm int RLUnCompReadByCallbackWrite16bit(const void *source, void *destination, unsigned int size, const void *stream)
{
    swi 0x15
    bx lr
}
