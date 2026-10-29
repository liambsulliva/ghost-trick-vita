void func_02011e1c(void *object);
extern int data_020a4ff0;

void *func_02011e08(void *object)
{
    func_02011e1c(object);
    *(int *)object = (int)&data_020a4ff0;
    return object;
}
