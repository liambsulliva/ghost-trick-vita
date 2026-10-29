void func_0201f268(void *object);
extern int data_020a46e8;

void *func_0201f254(void *object)
{
    func_0201f268(object);
    *(int *)object = (int)&data_020a46e8;
    return object;
}
