void func_02035ed8(void *object);
extern int data_020a63ac;

void *func_02035f3c(void *object)
{
    func_02035ed8(object);
    *(int *)object = (int)&data_020a63ac;
    return object;
}
