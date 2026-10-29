void func_02010eb0(void *child);

void func_02010e9c(char *object)
{
    void *child;
    child = *(void **)(object + 4024);
    if (child != 0) {
        func_02010eb0(child);
    }
}
