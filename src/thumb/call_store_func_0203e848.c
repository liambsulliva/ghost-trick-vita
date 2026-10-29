void func_0203e7d8(void *object);
extern int data_020a74b8;

void *func_0203e848(void *object)
{
    func_0203e7d8(object);
    *(int *)object = (int)&data_020a74b8;
    return object;
}
