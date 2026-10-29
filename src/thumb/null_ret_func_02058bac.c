int func_02058bc0(void *child);

int func_02058bac(char *object)
{
    void *child;
    child = *(void **)(object + 4);
    if (child != 0) {
        return func_02058bc0(child);
    }
    return 0;
}
