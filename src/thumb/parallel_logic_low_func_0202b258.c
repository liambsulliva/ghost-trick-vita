extern int func_0202b280(int entry);

int func_0202b258(int key,int table,unsigned int count)
{
    unsigned int index;
    int found;

    index = 0;
    goto test;
body:
    found = func_0202b280(table + (index << 4));
    if (key == found) {
        return index;
    }
    index = index + 1;
test:
    if (index < count) {
        goto body;
    }
    index = 0;
    return ~index;
}
