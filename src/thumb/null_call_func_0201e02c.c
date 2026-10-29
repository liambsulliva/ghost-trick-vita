void func_0201e03c(void *child);

void func_0201e02c(char *object)
{
    void *child;
    child = *(void **)(object + 0);
    if (child != 0) {
        func_0201e03c(child);
    }
}
