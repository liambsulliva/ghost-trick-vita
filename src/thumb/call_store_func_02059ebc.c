void func_0200a38c(void *object);
extern int data_020a2eac;

void *func_02059ebc(void *object)
{
    func_0200a38c(object);
    *(int *)object = (int)&data_020a2eac;
    return object;
}
