void *func_02031254(int size);
void func_02041bac(void *object);

void func_02041b98(void)
{
    void *object;
    object = func_02031254(32);
    if (object != 0) {
        func_02041bac(object);
    }
}
