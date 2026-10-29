void func_0200dc70(void *object);
extern int data_020a44a4;

void *func_0201db80(void *object)
{
    func_0200dc70(object);
    *(int *)object = (int)&data_020a44a4;
    return object;
}
