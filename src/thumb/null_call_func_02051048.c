void func_02050cb4(void *child);

void func_02051048(char *object)
{
    void *child;
    child = *(void **)(object + 48);
    if (child != 0) {
        func_02050cb4(child);
    }
}
