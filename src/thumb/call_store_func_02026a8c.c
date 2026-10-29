void func_02026a1c(void *object);
extern int data_020a4b34;

void *func_02026a8c(void *object)
{
    func_02026a1c(object);
    *(int *)object = (int)&data_020a4b34;
    return object;
}
