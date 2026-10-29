void func_0202694c(void *object);
extern int data_020a4b4c;

void *func_020269f4(void *object)
{
    func_0202694c(object);
    *(int *)object = (int)&data_020a4b4c;
    return object;
}
