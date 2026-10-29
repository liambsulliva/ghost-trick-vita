struct Table {
    unsigned int count;
    unsigned int entries[];
};

int func_02000d9c(int object,unsigned int index,int offset)
{
    int base;
    struct Table *table;
    unsigned int value;

    base = *(int *)(object + 0x98);
    table = (struct Table *)(base + offset);
    if (table == 0) {
        return 0;
    }
    if (index >= table->count) {
        return 0;
    }
    value = table->entries[index];
    if (value == 0) {
        return 0;
    }
    return base + value;
}
