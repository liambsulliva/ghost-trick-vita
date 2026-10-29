void func_0201db00(void *object);
extern int data_020a4444;

void *func_0201daec(void *object)
{
    func_0201db00(object);
    *(int *)object = (int)&data_020a4444;
    return object;
}
