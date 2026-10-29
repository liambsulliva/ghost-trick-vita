void func_02026a1c(void *object);
extern int data_020a4bf4;

void *func_02026a3c(void *object)
{
    func_02026a1c(object);
    *(int *)object = (int)&data_020a4bf4;
    return object;
}
