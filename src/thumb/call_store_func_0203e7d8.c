void func_0203e768(void *object);
extern int data_020a7448;

void *func_0203e7d8(void *object)
{
    func_0203e768(object);
    *(int *)object = (int)&data_020a7448;
    return object;
}
