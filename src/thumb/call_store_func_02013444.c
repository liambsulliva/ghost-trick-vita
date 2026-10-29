void func_02013398(void *object);
extern int data_020a32e8;

void *func_02013444(void *object)
{
    func_02013398(object);
    *(int *)object = (int)&data_020a32e8;
    return object;
}
