int func_0200f5e8(char *param_1)
{
    if (((((unsigned int)*(int *)(param_1 + 8)) << 16) >> 31) != 0) {
        return 1 << 22;
    }
    return *(int *)(param_1 + 0xda0) - *(int *)(param_1 + 0xddc);
}
