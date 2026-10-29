extern int func_0200ce78(int entry);

int func_0200ce5c(int object,unsigned int index)
{
    if (index >= *(unsigned int *)(object + 0x38)) {
        return 0;
    }
    return func_0200ce78(*(int *)(object + 0x3c) + index * 0x18);
}
