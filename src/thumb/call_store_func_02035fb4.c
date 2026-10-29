void func_02035fc8(void *object);
extern int data_020a6588;

void *func_02035fb4(void *object)
{
    func_02035fc8(object);
    *(int *)object = (int)&data_020a6588;
    return object;
}
