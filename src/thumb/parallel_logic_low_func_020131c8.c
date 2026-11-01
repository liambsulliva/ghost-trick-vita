void func_020131c8(unsigned char *param_1, int param_2)
{
    if (param_2 != 0) {
        param_1[0xd4] = param_1[0xd4] | 8;
    } else {
        param_1[0xd4] = param_1[0xd4] & ~8;
    }
}
