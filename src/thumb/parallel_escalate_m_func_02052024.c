void func_02052024(char *param_1, unsigned int param_2, unsigned int param_3)
{
    unsigned int value;

    if (param_2 == 0) {
        value = 0;
        *(unsigned int *)(param_1 + 0x28) = 0;
    } else {
        value = 0x100000;
        value *= param_3;
        value >>= 8;
        *(unsigned int *)(param_1 + 0x28) = value;
        value = value / param_2;
    }
    *(unsigned int *)(param_1 + 0x2c) = value;
}
