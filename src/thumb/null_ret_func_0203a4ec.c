int func_0203aa48(void *child);

int func_0203a4ec(char *object)
{
    void *child;
    child = *(void **)(object + 0);
    if (child != 0) {
        return func_0203aa48(child);
    }
    return 0;
}
