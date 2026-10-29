void func_0203a6c4(void *child);

void func_0203a4dc(char *object)
{
    void *child;
    child = *(void **)(object + 0);
    if (child != 0) {
        func_0203a6c4(child);
    }
}
