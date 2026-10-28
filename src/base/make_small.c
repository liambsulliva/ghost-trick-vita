void *func_02031254(int size);
void *func_02008b20(void *object);

void *func_02008b0c(void)
{
    void *object = func_02031254(4);
    if (object != 0) {
        object = func_02008b20(object);
    }
    return object;
}
