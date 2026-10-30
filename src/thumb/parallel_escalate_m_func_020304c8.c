void func_020304c8(int param_1)
{
    unsigned short *reg = (unsigned short *)0x04000060;
    unsigned int old = *reg;
    *reg = (0xffffcffdu & old) | (param_1 << 1);
}
