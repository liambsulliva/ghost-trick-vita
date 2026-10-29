void func_02026a1c(void *object);
extern int data_020a4a5c;

void *func_02026b2c(void *object)
{
    func_02026a1c(object);
    *(int *)object = (int)&data_020a4a5c;
    return object;
}
