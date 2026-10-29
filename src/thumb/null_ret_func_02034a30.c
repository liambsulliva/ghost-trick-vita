int func_02034990(void *child);

int func_02034a30(char *object)
{
    void *child;
    child = *(void **)(object + 36);
    if (child != 0) {
        return func_02034990(child);
    }
    return 0;
}
