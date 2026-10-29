void func_02013398(void *object);
extern int data_020a3328;

void *func_02013384(void *object)
{
    func_02013398(object);
    *(int *)object = (int)&data_020a3328;
    return object;
}
