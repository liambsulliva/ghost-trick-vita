void *func_02023d60(int size);
void func_020420cc(void *object);

void func_020420b8(void)
{
    void *object;
    object = func_02023d60(20);
    if (object != 0) {
        func_020420cc(object);
    }
}
