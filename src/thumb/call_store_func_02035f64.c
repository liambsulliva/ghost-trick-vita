void func_02035ed8(void *object);
extern int data_020a62e0;

void *func_02035f64(void *object)
{
    func_02035ed8(object);
    *(int *)object = (int)&data_020a62e0;
    return object;
}
