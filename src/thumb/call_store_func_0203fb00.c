void func_0202fe34(void *object);
extern int data_020a78b0;

void *func_0203fb00(void *object)
{
    func_0202fe34(object);
    *(int *)object = (int)&data_020a78b0;
    return object;
}
