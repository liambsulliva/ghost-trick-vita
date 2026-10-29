void func_0203a520(void *child);

void func_0203a510(char *object)
{
    void *child;
    child = *(void **)(object + 0);
    if (child != 0) {
        func_0203a520(child);
    }
}
