void func_02022d70(void *object);
extern int data_020a77d4;

void *func_0203fac4(void *object)
{
    func_02022d70(object);
    *(int *)object = (int)&data_020a77d4;
    return object;
}
