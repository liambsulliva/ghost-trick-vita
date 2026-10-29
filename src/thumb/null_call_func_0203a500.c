void func_0203aa5c(void *child);

void func_0203a500(char *object)
{
    void *child;
    child = *(void **)(object + 0);
    if (child != 0) {
        func_0203aa5c(child);
    }
}
