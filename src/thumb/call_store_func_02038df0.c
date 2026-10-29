void func_02038aa4(void *object);
extern int data_020a6df8;

void *func_02038df0(void *object)
{
    func_02038aa4(object);
    *(int *)object = (int)&data_020a6df8;
    return object;
}
