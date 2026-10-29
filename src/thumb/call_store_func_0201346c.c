void func_02013398(void *object);
extern int data_020a3288;

void *func_0201346c(void *object)
{
    func_02013398(object);
    *(int *)object = (int)&data_020a3288;
    return object;
}
