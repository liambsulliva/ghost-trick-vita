void func_0203fb00(void *object);
extern int data_020a7808;

void *func_0203fb34(void *object)
{
    func_0203fb00(object);
    *(int *)object = (int)&data_020a7808;
    return object;
}
