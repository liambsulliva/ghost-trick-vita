void func_02022d70(void *object);
extern int data_020a558c;

void *func_020318ac(void *object)
{
    func_02022d70(object);
    *(int *)object = (int)&data_020a558c;
    return object;
}
