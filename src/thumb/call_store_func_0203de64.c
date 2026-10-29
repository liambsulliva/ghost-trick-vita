void func_02013398(void *object);
extern int data_020a72b4;

void *func_0203de64(void *object)
{
    func_02013398(object);
    *(int *)object = (int)&data_020a72b4;
    return object;
}
