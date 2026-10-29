void func_02011774(void *object);
extern int data_020a5f1c;

void *func_02034fd0(void *object)
{
    func_02011774(object);
    *(int *)object = (int)&data_020a5f1c;
    return object;
}
