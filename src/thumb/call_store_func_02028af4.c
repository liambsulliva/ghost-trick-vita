void func_020293b8(void *object);
extern int data_020a4d60;

void *func_02028af4(void *object)
{
    func_020293b8(object);
    *(int *)object = (int)&data_020a4d60;
    return object;
}
