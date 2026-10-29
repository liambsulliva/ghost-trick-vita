void func_0203fb00(void *object);
extern int data_020a7840;

void *func_0203faec(void *object)
{
    func_0203fb00(object);
    *(int *)object = (int)&data_020a7840;
    return object;
}
