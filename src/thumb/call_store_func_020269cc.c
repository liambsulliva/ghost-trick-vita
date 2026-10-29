void func_0202694c(void *object);
extern int data_020a4bdc;

void *func_020269cc(void *object)
{
    func_0202694c(object);
    *(int *)object = (int)&data_020a4bdc;
    return object;
}
