void func_02011774(void *object);
extern int data_020b07c8;

void *func_02011760(void *object)
{
    func_02011774(object);
    *(int *)object = (int)&data_020b07c8;
    return object;
}
