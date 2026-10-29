void func_0200a38c(void *object);
extern int data_020a2e70;

void *func_0202f4b0(void *object)
{
    func_0200a38c(object);
    *(int *)object = (int)&data_020a2e70;
    return object;
}
