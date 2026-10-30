void func_02066410(int param_1, int param_2)
{
    if (param_2 != 0) {
        *(unsigned char *)(param_1 + 0x18c) = *(unsigned char *)(param_1 + 0x18c) | 8;
        *(unsigned char *)(param_1 + 0x18c) = *(unsigned char *)(param_1 + 0x18c) & ~0x10;
    }
    else {
        *(unsigned char *)(param_1 + 0x18c) = *(unsigned char *)(param_1 + 0x18c) & ~8;
        *(unsigned char *)(param_1 + 0x18c) = *(unsigned char *)(param_1 + 0x18c) & ~0x10;
    }
    return;
}
