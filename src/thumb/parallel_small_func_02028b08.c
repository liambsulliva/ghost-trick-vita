char *func_02028b08(char *param_1)
{
    char *p;

    p = param_1 + 0x39;
    *p = 0;
    p = param_1;
    *(int *)param_1 = 0;
    *(int *)(param_1 + 4) = 0;
    *(int *)(param_1 + 8) = 0;
    *(int *)(param_1 + 0xc) = 0;
    *(int *)(param_1 + 0x10) = 0;
    *(int *)(param_1 + 0x14) = 0;
    p = p + 0x3a;
    *p = 1;
    return param_1;
}
