void func_0201070c(void *child);

void func_020106f8(char *object)
{
    void *child;
    child = *(void **)(object + 3580);
    if (child != 0) {
        func_0201070c(child);
    }
}
