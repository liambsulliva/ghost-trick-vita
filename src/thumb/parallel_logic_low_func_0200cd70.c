extern int func_0200cc24(int entry);

int func_0200cd70(int object,unsigned int index)
{
    int table;

    table = *(int *)(object + 0x3c);
    if (table != 0 && index < *(unsigned int *)(object + 0x38)) {
        return func_0200cc24(table + index * 0x18);
    }
    return 0;
}
