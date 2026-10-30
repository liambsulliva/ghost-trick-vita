extern int func_020591f0();

int func_02058ab8(int param_1)
{
    if (*(int *)(param_1 + 4) != 0) {
        return func_020591f0(*(int *)(param_1 + 4), (unsigned int)*(int *)(param_1 + 0x14) >> 16);
    }
    return 0;
}
