void func_02022d70(void *object);
extern int data_020a7c8c;

void *func_02041bac(void *object)
{
    func_02022d70(object);
    *(int *)object = (int)&data_020a7c8c;
    return object;
}
