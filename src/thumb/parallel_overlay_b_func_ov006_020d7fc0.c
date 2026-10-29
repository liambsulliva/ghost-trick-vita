extern int func_0200b598(int, int, int);

int func_ov006_020d7fc0(int param_1, unsigned char param_2)
{
    int value;

    *(unsigned char *)(param_1 + 0xbc) = 1;
    *(unsigned char *)(param_1 + 0xbf) = param_2;
    value = *(int *)(param_1 + 0xd4);
    *(int *)(param_1 + 0xc8) = value;
    return func_0200b598(param_1 + 0x28, value, 0xf);
}
