void func_02061138(int param_1,int param_2)
{
    if (param_2 != 0) {
        *(unsigned char *)(param_1 + 0x7c) = *(unsigned char *)(param_1 + 0x7c) | 0x20;
        return;
    }
    *(unsigned char *)(param_1 + 0x7c) = *(unsigned char *)(param_1 + 0x7c) & ~0x20;
    return;
}
