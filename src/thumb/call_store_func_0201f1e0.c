void func_0201f1f4(void *object);
extern int data_020a4720;

void *func_0201f1e0(void *object)
{
    func_0201f1f4(object);
    *(int *)object = (int)&data_020a4720;
    return object;
}
