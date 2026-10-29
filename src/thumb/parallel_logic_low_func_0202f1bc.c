void func_0202f1bc(int *table,int a,int b,int c,int d)
{
    unsigned int count;
    unsigned int limit;
    int *slots;
    int *entry;
    int extra;

    limit = 1;
    count = table[0];
    limit = limit << 10;
    if (count < limit) {
        slots = table + 1;
        count = count << 4;
        *(int *)((char *)slots + count) = a;
        entry = (int *)((char *)slots + count);
        entry[1] = b;
        extra = d;
        entry[2] = c;
        entry[3] = extra;
        table[0] = table[0] + 1;
        extra = 0x4024;
        *(int *)((char *)table + extra) = *(int *)((char *)table + extra) + c;
    }
}
