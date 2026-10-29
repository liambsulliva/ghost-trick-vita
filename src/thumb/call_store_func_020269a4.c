void func_0202694c(void *object);
extern int data_020a4c6c;

void *func_020269a4(void *object)
{
    func_0202694c(object);
    *(int *)object = (int)&data_020a4c6c;
    return object;
}
