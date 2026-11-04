asm int Mod(int numerator, int divisor)
{
    swi 0x9
    add r0, r1, #0
    bx lr
}
