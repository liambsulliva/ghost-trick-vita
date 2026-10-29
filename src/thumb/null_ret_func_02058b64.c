int func_02058b78(void *child);

int func_02058b64(char *object)
{
    void *child;
    child = *(void **)(object + 4);
    if (child != 0) {
        return func_02058b78(child);
    }
    return 0;
}
