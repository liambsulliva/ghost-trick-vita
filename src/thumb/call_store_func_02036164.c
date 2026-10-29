void func_02035ed8(void *object);
extern int data_020a6654;

void *func_02036164(void *object)
{
    func_02035ed8(object);
    *(int *)object = (int)&data_020a6654;
    return object;
}
