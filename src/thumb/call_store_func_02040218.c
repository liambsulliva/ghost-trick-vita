void func_0204022c(void *object);
extern int data_020a7aec;

void *func_02040218(void *object)
{
    func_0204022c(object);
    *(int *)object = (int)&data_020a7aec;
    return object;
}
