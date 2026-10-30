void func_02059b18(int param_1, int param_2)
{
    int flag;
    unsigned int bits;

    flag = 1;
    if (param_2 == 0) {
        flag = 0;
    }
    bits = *(unsigned char *)(param_1 + 0x12) & ~4;
    bits |= (((unsigned int)flag) << 31) >> 29;
    *(unsigned char *)(param_1 + 0x12) = bits;
}
