void func_020125d4(unsigned char *param_1, unsigned int param_2)
{
    unsigned char *slot;

    if (param_2 == 0) {
        goto store;
    }
    slot = param_1 + 0x5d;
    if (param_2 <= *slot) {
        return;
    }
store:
    param_1[0x5d] = param_2;
}
