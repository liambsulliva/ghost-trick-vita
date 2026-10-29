void func_02013398(void *object);
extern int data_020a5d08;

void *func_02034f50(void *object)
{
    func_02013398(object);
    *(int *)object = (int)&data_020a5d08;
    return object;
}
