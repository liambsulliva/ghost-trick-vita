void func_02011774(void *object);
extern int data_020b04b8;

void *func_02065b44(void *object)
{
    func_02011774(object);
    *(int *)object = (int)&data_020b04b8;
    return object;
}
