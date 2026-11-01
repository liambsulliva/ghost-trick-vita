int func_0200f178(int param_1)
{
    unsigned int flags;

    flags = *(unsigned int *)(param_1 + 8);
    if (((flags << 16) >> 31) != 0) {
        return 0x400000;
    }
    return *(int *)(param_1 + 0xd9c);
}
