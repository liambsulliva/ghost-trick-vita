void func_0202694c(void *object);
extern int data_020a4bac;

void *func_02026938(void *object)
{
    func_0202694c(object);
    *(int *)object = (int)&data_020a4bac;
    return object;
}
