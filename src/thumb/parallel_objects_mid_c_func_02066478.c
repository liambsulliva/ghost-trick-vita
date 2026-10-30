void func_02066478(int param_1, int param_2)
{
    if (param_2 != 0) {
        *(unsigned char *)(param_1 + 0x18c) = *(unsigned char *)(param_1 + 0x18c) | 0x40;
        return;
    }
    *(unsigned char *)(param_1 + 0x18c) = *(unsigned char *)(param_1 + 0x18c) & ~0x40;
    return;
}
