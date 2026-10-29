void func_0205d414(void *child);

void func_0205e294(char *object)
{
    void *child;
    child = *(void **)(object + 128);
    if (child != 0) {
        func_0205d414(child);
    }
}
