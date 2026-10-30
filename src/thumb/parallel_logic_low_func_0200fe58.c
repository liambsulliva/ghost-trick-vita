int func_0200fe58(unsigned int *param_1)
{
    unsigned int uVar1;

    uVar1 = (param_1[2] << 8) >> 24;
    if (uVar1 == 2) {
        return 1;
    }
    if (uVar1 == 3) {
        return 1;
    }
    return 0;
}
