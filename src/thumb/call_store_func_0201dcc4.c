void func_020145dc(void *object);
extern int data_020e47d0;

void *func_0201dcc4(void *object)
{
    func_020145dc(object);
    *(int *)object = (int)&data_020e47d0;
    return object;
}
