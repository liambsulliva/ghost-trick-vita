asm int Div(int numerator, int divisor)
{
    swi 0x9
    bx lr
}
