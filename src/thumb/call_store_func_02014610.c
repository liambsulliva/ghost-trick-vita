void func_020145dc(void *object);
extern int data_020a33c0;

void *func_02014610(void *object)
{
    func_020145dc(object);
    *(int *)object = (int)&data_020a33c0;
    return object;
}
