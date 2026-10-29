extern int func_0202d684(int object);

int func_0202d68c(int object,int index)
{
    int *table;

    table = *(int **)(object + 0xe0);
    if (table != 0) {
        return *(int *)((unsigned char *)table + index * 8);
    }
    return func_0202d684(object);
}
