void func_02026a1c(void *object);
extern int data_020a4abc;

void *func_02026ac8(void *object)
{
    func_02026a1c(object);
    *(int *)object = (int)&data_020a4abc;
    return object;
}
