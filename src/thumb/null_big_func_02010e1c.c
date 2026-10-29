void func_02010e30(void *child);

void func_02010e1c(char *object)
{
    void *child;
    child = *(void **)(object + 4024);
    if (child != 0) {
        func_02010e30(child);
    }
}
