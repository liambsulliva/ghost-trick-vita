void *func_02023d60(int size);
void func_02042b50(void *object);

void func_02042704(void)
{
    void *object;
    object = func_02023d60(24);
    if (object != 0) {
        func_02042b50(object);
    }
}
