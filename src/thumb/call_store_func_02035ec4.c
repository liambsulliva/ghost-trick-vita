void func_02035ed8(void *object);
extern int data_020a65cc;

void *func_02035ec4(void *object)
{
    func_02035ed8(object);
    *(int *)object = (int)&data_020a65cc;
    return object;
}
