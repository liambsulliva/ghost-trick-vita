void func_02056ac8(void *object);
extern int data_020a69ac;

void *func_0203706c(void *object)
{
    func_02056ac8(object);
    *(int *)object = (int)&data_020a69ac;
    return object;
}
