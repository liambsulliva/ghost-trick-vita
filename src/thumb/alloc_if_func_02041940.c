void *func_02031254(int size);
void func_02041954(void *object);

void func_02041940(void)
{
    void *object;
    object = func_02031254(4);
    if (object != 0) {
        func_02041954(object);
    }
}
