void func_02013398(void *object);
extern int data_020a7ce4;

void *func_02041dac(void *object)
{
    func_02013398(object);
    *(int *)object = (int)&data_020a7ce4;
    return object;
}
