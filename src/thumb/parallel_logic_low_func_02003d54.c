extern char data_020b18a0;

int func_02003d54(unsigned int param_1)
{
    int i;
    char *base;
    char *entry;

    base = *(char **)(&data_020b18a0 + 0x10);
    for (i = 0; i < 16; i++) {
        entry = base + i * 0x4c;
        if (*(unsigned char *)(entry + 0x15ce) == param_1) {
            return *(int *)(entry + 0x1604);
        }
    }
    return -1;
}
