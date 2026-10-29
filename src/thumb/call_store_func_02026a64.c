void func_02026a1c(void *object);
extern int data_020a4b7c;

void *func_02026a64(void *object)
{
    func_02026a1c(object);
    *(int *)object = (int)&data_020a4b7c;
    return object;
}
