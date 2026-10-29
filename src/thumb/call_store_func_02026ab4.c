void func_02026a1c(void *object);
extern int data_020a4ad4;

void *func_02026ab4(void *object)
{
    func_02026a1c(object);
    *(int *)object = (int)&data_020a4ad4;
    return object;
}
