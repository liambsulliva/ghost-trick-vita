void func_02022d70(void *object);
extern int data_020a7d04;

void *func_02041d84(void *object)
{
    func_02022d70(object);
    *(int *)object = (int)&data_020a7d04;
    return object;
}
