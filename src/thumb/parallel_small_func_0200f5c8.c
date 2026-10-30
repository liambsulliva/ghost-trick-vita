int func_0200f5c8(char *param_1)
{
    if (((((unsigned int)*(int *)(param_1 + 8)) << 16) >> 31) != 0) {
        return 1 << 22;
    }
    return *(int *)(param_1 + 0xd9c) - *(int *)(param_1 + 0xdd8);
}
