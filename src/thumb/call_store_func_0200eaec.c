void func_0200a38c(void *object);
extern int data_020a2df8;

void *func_0200eaec(void *object)
{
    func_0200a38c(object);
    *(int *)object = (int)&data_020a2df8;
    return object;
}
