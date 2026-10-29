void func_0202fe34(void *object);
extern int data_020ad3c8;

void *func_02054c7c(void *object)
{
    func_0202fe34(object);
    *(int *)object = (int)&data_020ad3c8;
    return object;
}
