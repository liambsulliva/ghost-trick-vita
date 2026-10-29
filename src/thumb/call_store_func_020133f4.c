void func_02013398(void *object);
extern int data_020a3368;

void *func_020133f4(void *object)
{
    func_02013398(object);
    *(int *)object = (int)&data_020a3368;
    return object;
}
