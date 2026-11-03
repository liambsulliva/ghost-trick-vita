void func_020668dc(unsigned char *param_1, unsigned int param_2)
{
    int mask;
    int value;

    mask = 4;
    value = param_1[0x18c];
    value = value & ~mask;
    param_1[0x18c] = value | ((param_2 << 31) >> 29);
}
