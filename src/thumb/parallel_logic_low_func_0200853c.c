void func_0200853c(char *param_1, unsigned short param_2)
{
    int flags;
    int one;

    *(unsigned short *)(*(int *)(param_1 + 0x14) + 0x50) = param_2;
    flags = param_1[0x12];
    if (((((unsigned int)flags) << 29) >> 31) == 0) {
        one = 1;
        flags = flags & ~one;
        one = 1;
        flags = one | flags;
        param_1[0x12] = flags;
        flags = param_1[0x12];
        one = 2;
        flags = flags & ~one;
        param_1[0x12] = flags;
    }
}
