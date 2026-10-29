void func_0200a38c(void *object);
extern int data_020a2e20;

void *func_0200eb28(void *object)
{
    func_0200a38c(object);
    *(int *)object = (int)&data_020a2e20;
    return object;
}
