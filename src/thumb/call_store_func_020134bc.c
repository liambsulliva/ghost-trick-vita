void func_02013398(void *object);
extern int data_020a3348;

void *func_020134bc(void *object)
{
    func_02013398(object);
    *(int *)object = (int)&data_020a3348;
    return object;
}
