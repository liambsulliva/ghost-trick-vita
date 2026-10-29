extern int func_ov003_020d4d50(int, int, int);

int func_ov003_020d5360(int param_1)
{
    int value;

    value = *(int *)(param_1 + 0x174);
    return func_ov003_020d4d50(param_1, 0x1c - value, 1);
}
