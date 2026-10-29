int func_020164c4(void *child);

int func_020164b0(char *object)
{
    void *child;
    child = *(void **)(object + 4);
    if (child != 0) {
        return func_020164c4(child);
    }
    return 0;
}
