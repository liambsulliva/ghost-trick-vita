void func_02022d70(void *object);
extern int data_020a72d4;

void *func_0203de3c(void *object)
{
    func_02022d70(object);
    *(int *)object = (int)&data_020a72d4;
    return object;
}
