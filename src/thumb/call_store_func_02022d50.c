void func_02022d70(void *object);
extern int data_020a496c;

void *func_02022d50(void *object)
{
    func_02022d70(object);
    *(int *)object = (int)&data_020a496c;
    return object;
}
