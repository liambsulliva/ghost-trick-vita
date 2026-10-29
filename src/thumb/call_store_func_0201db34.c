void func_0201db00(void *object);
extern int data_020a4464;

void *func_0201db34(void *object)
{
    func_0201db00(object);
    *(int *)object = (int)&data_020a4464;
    return object;
}
