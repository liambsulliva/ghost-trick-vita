void func_02053d14(void *object);
extern int data_020ad0c8;

void *func_02053c58(void *object)
{
    func_02053d14(object);
    *(int *)object = (int)&data_020ad0c8;
    return object;
}
