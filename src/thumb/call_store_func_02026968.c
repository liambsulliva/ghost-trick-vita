void func_02026938(void *object);
extern int data_020a4c24;

void *func_02026968(void *object)
{
    func_02026938(object);
    *(int *)object = (int)&data_020a4c24;
    return object;
}
