void func_02022d70(void *object);
extern int data_020a78f4;

void *func_0203fffc(void *object)
{
    func_02022d70(object);
    *(int *)object = (int)&data_020a78f4;
    return object;
}
