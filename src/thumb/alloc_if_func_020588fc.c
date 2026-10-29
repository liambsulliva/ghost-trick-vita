void *func_02058a10(int size);
void func_02058910(void *object);

void func_020588fc(void)
{
    void *object;
    object = func_02058a10(24);
    if (object != 0) {
        func_02058910(object);
    }
}
