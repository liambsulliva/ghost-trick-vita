void func_02026a1c(void *object);
extern int data_020a4b1c;

void *func_02026aa0(void *object)
{
    func_02026a1c(object);
    *(int *)object = (int)&data_020a4b1c;
    return object;
}
