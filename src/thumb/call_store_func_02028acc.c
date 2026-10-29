void func_020293b8(void *object);
extern int data_020a4ee8;

void *func_02028acc(void *object)
{
    func_020293b8(object);
    *(int *)object = (int)&data_020a4ee8;
    return object;
}
