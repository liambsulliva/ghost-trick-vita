typedef int code();

int func_0202bbc0(void *owner)
{
    int sum = 1;
    void *p = *(void **)((char *)owner + 0x74);
    while (p != 0) {
        void *cur = p;
        p = *(void **)((char *)cur + 0x78);
        sum = sum + ((code *)(*(void ***)cur)[5])(cur);
    }
    return sum;
}
