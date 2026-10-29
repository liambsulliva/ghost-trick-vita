void func_0203d504(void *object);
extern int data_020a718c;

void *func_0203c3e0(void *object)
{
    func_0203d504(object);
    *(int *)object = (int)&data_020a718c;
    return object;
}
