extern int func_0202d688(int object);

int func_0202d6a4(int object,int index)
{
    int *table;

    table = *(int **)(object + 0xe0);
    if (table != 0) {
        return *(int *)((unsigned char *)table + index * 8 + 4);
    }
    return func_0202d688(object);
}
