void func_02053d14(void *object);
extern int data_020ad040;

void *func_02053c80(void *object)
{
    func_02053d14(object);
    *(int *)object = (int)&data_020ad040;
    return object;
}
