int func_02050cd4(void *child);

int func_02050cc0(char *object)
{
    void *child;
    child = *(void **)(object + 56);
    if (child != 0) {
        return func_02050cd4(child);
    }
    return 0;
}
