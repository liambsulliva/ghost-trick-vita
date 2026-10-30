void func_02066444(unsigned char *param_1, unsigned int param_2)
{
    int mask;
    int value;

    mask = 0x20;
    value = param_1[0x18c];
    value = value & ~mask;
    param_1[0x18c] = value | ((param_2 << 31) >> 26);
}
