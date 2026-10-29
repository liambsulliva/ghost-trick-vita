void func_020152c0(void *child);

void func_02014a7c(char *object)
{
    void *child;
    child = *(void **)(object + 4);
    if (child != 0) {
        func_020152c0(child);
    }
}
