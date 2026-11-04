asm int Sqrt(unsigned int value)
{
    swi 0xd
    bx lr
}
