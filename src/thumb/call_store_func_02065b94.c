void func_02011774(void *object);
extern int data_020b0de8;

void *func_02065b94(void *object)
{
    func_02011774(object);
    *(int *)object = (int)&data_020b0de8;
    return object;
}
