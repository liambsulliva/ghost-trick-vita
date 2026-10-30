int func_020108ec(int *param_1)
{
    unsigned int flags;
    int *owner;

    flags = (unsigned int)param_1[3];
    if (((flags << 2) >> 31) != 0) {
        return 0;
    }
    param_1[3] = (int)(flags | 0x20000000);
    owner = (int *)param_1[0x380];
    if (owner != 0) {
        ((void (*)(int *))(*(int *)(*owner + 0xd8)))(owner);
    }
    owner = (int *)param_1[0x37f];
    if (owner != 0) {
        ((void (*)(int *))(*(int *)(*owner + 0xd8)))(owner);
    }
    return 1;
}
