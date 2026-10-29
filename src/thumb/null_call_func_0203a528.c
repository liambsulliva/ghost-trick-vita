void func_0203a538(void *child);

void func_0203a528(char *object)
{
    void *child;
    child = *(void **)(object + 0);
    if (child != 0) {
        func_0203a538(child);
    }
}
