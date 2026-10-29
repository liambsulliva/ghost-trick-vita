void func_0201e0d4(void *child);

void func_0201e0c4(char *object)
{
    void *child;
    child = *(void **)(object + 0);
    if (child != 0) {
        func_0201e0d4(child);
    }
}
