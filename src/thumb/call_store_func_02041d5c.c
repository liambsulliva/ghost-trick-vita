void func_02022d70(void *object);
extern int data_020a7d38;

void *func_02041d5c(void *object)
{
    func_02022d70(object);
    *(int *)object = (int)&data_020a7d38;
    return object;
}
