void func_020101d0(unsigned char *param_1, int param_2, unsigned int param_3)
{
    unsigned char low;
    unsigned int bits;
    unsigned int flags;

    param_1[0xbf] = param_2;
    flags = param_1[0xde];
    flags = flags & ~0xc0;
    low = param_3;
    bits = (((unsigned int)low) << 30) >> 24;
    flags = flags | bits;
    param_1[0xde] = flags;
}
