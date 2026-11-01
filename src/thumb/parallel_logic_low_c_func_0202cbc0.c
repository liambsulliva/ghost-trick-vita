typedef int code3(void *node, void *a, void *b);

int func_0202cbc0(void *owner, void *a, void *b)
{
    void *p = *(void **)((char *)owner + 0x74);
    while (p != 0) {
        void *cur = p;
        p = *(void **)((char *)cur + 0x78);
        if (((code3 *)(*(void ***)cur)[16])(cur, a, b) != 0) {
            return 1;
        }
    }
    return 0;
}
