void func_0200c524(void *object);
extern int data_020a45a4;

void *func_0201f1f4(void *object)
{
    func_0200c524(object);
    *(int *)object = (int)&data_020a45a4;
    return object;
}
