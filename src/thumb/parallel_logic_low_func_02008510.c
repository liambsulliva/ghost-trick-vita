unsigned int func_02008510(char *param_1)
{
    unsigned short *table;
    unsigned int index;
    unsigned int value;

    table = *(unsigned short **)(param_1 + 4);
    if (table == 0) {
        return 1;
    }
    index = *(unsigned int *)(param_1 + 8);
    value = table[index];
    if (value == 0xfffe || (((unsigned int)*(unsigned char *)(param_1 + 0x12) << 27) >> 31)) {
        return 1;
    }
    return 0;
}
