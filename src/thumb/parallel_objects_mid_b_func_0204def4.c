void func_0204def4(int a, int b)
{
    unsigned short lo = (a << 8) >> 16;
    unsigned short hi = (b << 8) >> 16;
    *(volatile unsigned int *)0x04000488 = (unsigned int)lo | ((unsigned int)hi << 16);
}
