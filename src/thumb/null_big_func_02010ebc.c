void func_02010ed0(void *child);

void func_02010ebc(char *object)
{
    void *child;
    child = *(void **)(object + 4024);
    if (child != 0) {
        func_02010ed0(child);
    }
}
