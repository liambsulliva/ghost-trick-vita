int func_0202e344(unsigned char *obj, int idx)
{
    int base;
    int *table;
    if (idx < 0) {
        return 0;
    }
    if ((unsigned int)idx >= *(unsigned int *)(obj + 0x158)) {
        return 0;
    }
    base = *(int *)(obj + 0x160);
    table = *(int **)(obj + 0x15c);
    return base + table[idx];
}
