void func_02013398(void *object);
extern int data_020a3308;

void *func_0201341c(void *object)
{
    func_02013398(object);
    *(int *)object = (int)&data_020a3308;
    return object;
}
