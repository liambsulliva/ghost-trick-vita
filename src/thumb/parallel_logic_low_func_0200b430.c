void func_0200b430(unsigned char *param_1, int param_2, unsigned int param_3)
{
    unsigned int flags;

    if (param_2 != 0) {
        flags = param_1[0x1c];
        flags = flags | 8;
        param_1[0x1c] = flags;
        flags = param_1[0x1c];
        flags = flags & ~0xf0;
        flags = flags | (((param_3 << 28) >> 24));
        param_1[0x1c] = flags;
    } else {
        flags = param_1[0x1c];
        flags = flags & ~8;
        param_1[0x1c] = flags;
        flags = param_1[0x1c];
        flags = flags & ~0xf0;
        param_1[0x1c] = flags;
    }
}
