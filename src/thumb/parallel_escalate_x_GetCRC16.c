asm int GetCRC16(unsigned short crc, const void *data, unsigned int size)
{
    swi 0xe
    bx lr
}
