void func_02022d70(void *object);
extern int data_020a571c;

void *func_02031c3c(void *object)
{
    func_02022d70(object);
    *(int *)object = (int)&data_020a571c;
    return object;
}
