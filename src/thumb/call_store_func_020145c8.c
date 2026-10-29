void func_020145dc(void *object);
extern int data_020a33e0;

void *func_020145c8(void *object)
{
    func_020145dc(object);
    *(int *)object = (int)&data_020a33e0;
    return object;
}
