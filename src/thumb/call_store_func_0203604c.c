void func_02035ed8(void *object);
extern int data_020a64bc;

void *func_0203604c(void *object)
{
    func_02035ed8(object);
    *(int *)object = (int)&data_020a64bc;
    return object;
}
