void func_0201e0ac(void *child);

void func_0201e09c(char *object)
{
    void *child;
    child = *(void **)(object + 0);
    if (child != 0) {
        func_0201e0ac(child);
    }
}
