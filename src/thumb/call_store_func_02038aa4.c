void func_02056ac8(void *object);
extern int data_020a69ac;

void *func_02038aa4(void *object)
{
    func_02056ac8(object);
    *(int *)object = (int)&data_020a69ac;
    return object;
}
