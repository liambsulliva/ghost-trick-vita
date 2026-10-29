void func_02013398(void *object);
extern int data_020a3388;

void *func_02013494(void *object)
{
    func_02013398(object);
    *(int *)object = (int)&data_020a3388;
    return object;
}
