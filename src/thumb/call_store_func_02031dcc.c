void func_02022d70(void *object);
extern int data_020a56e8;

void *func_02031dcc(void *object)
{
    func_02022d70(object);
    *(int *)object = (int)&data_020a56e8;
    return object;
}
