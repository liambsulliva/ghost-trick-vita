void func_0201e084(void *child);

void func_0201e074(char *object)
{
    void *child;
    child = *(void **)(object + 0);
    if (child != 0) {
        func_0201e084(child);
    }
}
