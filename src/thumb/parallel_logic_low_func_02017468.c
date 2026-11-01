void func_02017468(unsigned int *param_1, unsigned int param_2, unsigned int param_3)
{
    if ((param_2 != 0x100) && (param_2 != 0x300)) {
        *param_1 = param_2 | (*param_1 & 0xc1fffcff);
        return;
    }
    *param_1 = param_2 | (*param_1 & 0xc1fffcff) | (param_3 << 25);
    return;
}
