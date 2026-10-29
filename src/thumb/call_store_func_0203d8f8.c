void func_02022d70(void *object);
extern int data_020a7124;

void *func_0203d8f8(void *object)
{
    func_02022d70(object);
    *(int *)object = (int)&data_020a7124;
    return object;
}
