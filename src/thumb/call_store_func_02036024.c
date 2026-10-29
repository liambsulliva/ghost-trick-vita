void func_02035ed8(void *object);
extern int data_020a66dc;

void *func_02036024(void *object)
{
    func_02035ed8(object);
    *(int *)object = (int)&data_020a66dc;
    return object;
}
