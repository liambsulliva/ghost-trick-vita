void func_02015ba8(void *child);

void func_02015b98(char *object)
{
    void *child;
    child = *(void **)(object + 12);
    if (child != 0) {
        func_02015ba8(child);
    }
}
