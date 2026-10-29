void func_0202694c(void *object);
extern int data_020a4aec;

void *func_02026af0(void *object)
{
    func_0202694c(object);
    *(int *)object = (int)&data_020a4aec;
    return object;
}
