void func_02013398(void *object);
extern int data_020a7338;

void *func_0203e2d0(void *object)
{
    func_02013398(object);
    *(int *)object = (int)&data_020a7338;
    return object;
}
