void func_0203e290(void *object);
extern int data_020a7390;

void *func_0203e2f8(void *object)
{
    func_0203e290(object);
    *(int *)object = (int)&data_020a7390;
    return object;
}
