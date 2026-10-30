int func_0200acc8(signed char *param_1)
{
    int v;

    param_1[4] = param_1[4] + param_1[5];
    if (param_1[4] < 0) {
        param_1[4] = 0;
        param_1[5] = 1;
    }
    if (param_1[4] > 0x4c) {
        param_1[4] = 0x4c;
        param_1[5] = -1;
    }
    v = param_1[4];
    if (v > 0x38) {
        v = 0x38;
    }
    return (signed char)((v << 4) / 0x38 + 0xf);
}
