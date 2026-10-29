void func_02011774(void *object);
extern int data_020b0f70;

void *func_02065b6c(void *object)
{
    func_02011774(object);
    *(int *)object = (int)&data_020b0f70;
    return object;
}
