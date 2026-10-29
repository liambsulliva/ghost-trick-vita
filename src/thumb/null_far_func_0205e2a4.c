void func_0205d490(void *child);

void func_0205e2a4(char *object)
{
    void *child;
    child = *(void **)(object + 128);
    if (child != 0) {
        func_0205d490(child);
    }
}
