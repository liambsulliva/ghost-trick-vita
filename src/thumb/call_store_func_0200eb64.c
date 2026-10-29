void func_0200a38c(void *object);
extern int data_020a2e98;

void *func_0200eb64(void *object)
{
    func_0200a38c(object);
    *(int *)object = (int)&data_020a2e98;
    return object;
}
