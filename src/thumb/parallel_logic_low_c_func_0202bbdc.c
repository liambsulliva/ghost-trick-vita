typedef int code3(void *node, int v, int *counter);

int func_0202bbdc(void *owner, int v, int *counter)
{
    int c = *counter;
    int r;
    void *p;
    if (v == c) {
        return (int)owner;
    }
    *counter = c + 1;
    p = *(void **)((char *)owner + 0x74);
    while (p != 0) {
        void *cur = p;
        p = *(void **)((char *)cur + 0x78);
        r = ((code3 *)(*(void ***)cur)[6])(cur, v, counter);
        if (r != 0) {
            return r;
        }
    }
    return 0;
}
