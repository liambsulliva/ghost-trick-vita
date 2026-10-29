void func_0201e534(void *object);
extern int data_020a45dc;

void *func_0201e540(void *object)
{
    func_0201e534(object);
    *(int *)object = (int)&data_020a45dc;
    return object;
}
