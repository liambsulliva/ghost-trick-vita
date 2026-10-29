void func_02026938(void *object);
extern int data_020a4c54;

void *func_02026924(void *object)
{
    func_02026938(object);
    *(int *)object = (int)&data_020a4c54;
    return object;
}
