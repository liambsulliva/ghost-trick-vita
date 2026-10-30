int func_0206c374(const signed char *param_1, int param_2)
{
    int bVar1;
    unsigned int param_3;

    while ((bVar1 = param_1[param_2]) != 0 && (param_3 = (unsigned char)bVar1) != 0x2f && param_3 != 0x5c)
    {
        param_3 = (unsigned int)(((unsigned char)bVar1 ^ 0x20) - 0xa1) < 0x3c;
        param_2 = param_2 + 1 + param_3;
    }
    return param_2;
}
