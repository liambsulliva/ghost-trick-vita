void func_0200a38c(void *object);
extern int data_020a2e5c;

void *func_0200eb50(void *object)
{
    func_0200a38c(object);
    *(int *)object = (int)&data_020a2e5c;
    return object;
}
