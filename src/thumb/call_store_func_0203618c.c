void func_02035ed8(void *object);
extern int data_020a6610;

void *func_0203618c(void *object)
{
    func_02035ed8(object);
    *(int *)object = (int)&data_020a6610;
    return object;
}
