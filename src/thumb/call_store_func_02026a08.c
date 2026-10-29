void func_02026a1c(void *object);
extern int data_020a4c0c;

void *func_02026a08(void *object)
{
    func_02026a1c(object);
    *(int *)object = (int)&data_020a4c0c;
    return object;
}
