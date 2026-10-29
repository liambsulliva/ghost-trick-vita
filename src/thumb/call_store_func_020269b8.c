void func_0202694c(void *object);
extern int data_020a4c3c;

void *func_020269b8(void *object)
{
    func_0202694c(object);
    *(int *)object = (int)&data_020a4c3c;
    return object;
}
