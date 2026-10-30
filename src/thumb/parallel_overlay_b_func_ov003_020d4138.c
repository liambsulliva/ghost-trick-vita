extern int func_ov003_020d414c(int);

int func_ov003_020d4138(int param_1)
{
    int count = func_ov003_020d414c((~param_1) & (param_1 - 1));
    return 0x20 - count;
}
