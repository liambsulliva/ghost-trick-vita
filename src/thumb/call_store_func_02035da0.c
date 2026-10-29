void func_02022d70(void *object);
extern int data_020a6140;

void *func_02035da0(void *object)
{
    func_02022d70(object);
    *(int *)object = (int)&data_020a6140;
    return object;
}
