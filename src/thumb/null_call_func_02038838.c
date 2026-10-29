void func_0203a528(void *child);

void func_02038838(char *object)
{
    void *child;
    child = *(void **)(object + 12);
    if (child != 0) {
        func_0203a528(child);
    }
}
