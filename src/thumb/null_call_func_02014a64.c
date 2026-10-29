void func_02014a74(void *child);

void func_02014a64(char *object)
{
    void *child;
    child = *(void **)(object + 4);
    if (child != 0) {
        func_02014a74(child);
    }
}
