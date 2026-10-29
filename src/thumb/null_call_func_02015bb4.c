void func_02015bc4(void *child);

void func_02015bb4(char *object)
{
    void *child;
    child = *(void **)(object + 12);
    if (child != 0) {
        func_02015bc4(child);
    }
}
