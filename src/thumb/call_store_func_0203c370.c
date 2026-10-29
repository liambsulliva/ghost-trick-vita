void func_0203c414(void *object);
extern int data_020a7228;

void *func_0203c370(void *object)
{
    func_0203c414(object);
    *(int *)object = (int)&data_020a7228;
    return object;
}
