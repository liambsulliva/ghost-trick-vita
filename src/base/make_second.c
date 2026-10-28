void *func_02031254(int size);
void *func_02008b48(void *object);

void *func_02008b34(void)
{
    void *object = func_02031254(4);
    if (object != 0) {
        object = func_02008b48(object);
    }
    return object;
}
