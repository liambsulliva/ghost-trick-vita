void func_02035ed8(void *object);
extern int data_020a6258;

void *func_0203613c(void *object)
{
    func_02035ed8(object);
    *(int *)object = (int)&data_020a6258;
    return object;
}
