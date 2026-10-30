void func_0200ee34(char *param_1, unsigned int param_2, unsigned int param_3, unsigned int param_4)
{
    unsigned short value;
    int i;

    value = param_2 | (param_3 << 5) | (param_4 << 10);
    for (i = 0; i < 4; i = i + 1) {
        *(unsigned short *)(param_1 + i * 2 + 0x9c) = value;
    }
}
