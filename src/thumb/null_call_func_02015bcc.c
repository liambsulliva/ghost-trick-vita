void func_02015bdc(void *child);

void func_02015bcc(char *object)
{
    void *child;
    child = *(void **)(object + 12);
    if (child != 0) {
        func_02015bdc(child);
    }
}
