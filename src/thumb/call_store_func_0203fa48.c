void func_02022d70(void *object);
extern int data_020a7770;

void *func_0203fa48(void *object)
{
    func_02022d70(object);
    *(int *)object = (int)&data_020a7770;
    return object;
}
