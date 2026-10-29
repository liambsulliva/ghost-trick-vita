void func_02035ed8(void *object);
extern int data_020a63f0;

void *func_02035ffc(void *object)
{
    func_02035ed8(object);
    *(int *)object = (int)&data_020a63f0;
    return object;
}
