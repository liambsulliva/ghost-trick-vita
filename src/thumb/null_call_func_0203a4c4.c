void func_0203a4d4(void *child);

void func_0203a4c4(char *object)
{
    void *child;
    child = *(void **)(object + 0);
    if (child != 0) {
        func_0203a4d4(child);
    }
}
