void func_0200a38c(void *object);
extern int data_020a2e48;

void *func_0200eb3c(void *object)
{
    func_0200a38c(object);
    *(int *)object = (int)&data_020a2e48;
    return object;
}
