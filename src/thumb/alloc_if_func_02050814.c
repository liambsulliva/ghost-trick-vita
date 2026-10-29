void *func_02031254(int size);
void func_02051648(void *object);

void func_02050814(void)
{
    void *object;
    object = func_02031254(56);
    if (object != 0) {
        func_02051648(object);
    }
}
