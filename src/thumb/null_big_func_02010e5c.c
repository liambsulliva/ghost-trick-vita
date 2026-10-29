void func_02010e70(void *child);

void func_02010e5c(char *object)
{
    void *child;
    child = *(void **)(object + 4024);
    if (child != 0) {
        func_02010e70(child);
    }
}
