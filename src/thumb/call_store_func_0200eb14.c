void func_0200a38c(void *object);
extern int data_020a2e0c;

void *func_0200eb14(void *object)
{
    func_0200a38c(object);
    *(int *)object = (int)&data_020a2e0c;
    return object;
}
