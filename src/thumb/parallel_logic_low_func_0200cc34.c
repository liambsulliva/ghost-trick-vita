int func_0200cc34(int *param_1, unsigned int param_2)
{
    char *entry;
    unsigned int index;
    unsigned int count;

    entry = (char *)param_1[15];
    index = 0;
    count = param_1[14];
    while (index < count) {
        if ((((*(unsigned int *)(entry + 0x10)) << 6) >> 16) == param_2) {
            return index;
        }
        index = index + 1;
        entry = entry + 0x18;
    }
    return -1;
}
