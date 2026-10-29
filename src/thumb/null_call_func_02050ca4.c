void func_02050cb4(void *child);

void func_02050ca4(char *object)
{
    void *child;
    child = *(void **)(object + 56);
    if (child != 0) {
        func_02050cb4(child);
    }
}
