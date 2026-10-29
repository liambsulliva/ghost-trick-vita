void func_02022d70(void *object);
extern int data_020a5750;

void *func_02031c8c(void *object)
{
    func_02022d70(object);
    *(int *)object = (int)&data_020a5750;
    return object;
}
