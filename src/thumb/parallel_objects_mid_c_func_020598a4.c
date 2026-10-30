void func_020598a4(int param_1, int param_2)
{
    unsigned char flags;

    flags = *(unsigned char *)(param_1 + 0x12);
    *(int *)(param_1 + 0x28) = param_2;
    *(unsigned char *)(param_1 + 0x12) = flags | 0x20;
    *(unsigned char *)(param_2 + 0x12) = *(unsigned char *)(param_2 + 0x12) | 0x20;
}
