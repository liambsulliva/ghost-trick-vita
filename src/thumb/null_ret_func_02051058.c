int func_02050cd4(void *child);

int func_02051058(char *object)
{
    void *child;
    child = *(void **)(object + 48);
    if (child != 0) {
        return func_02050cd4(child);
    }
    return 0;
}
