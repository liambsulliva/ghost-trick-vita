void func_0200a38c(void *object);
extern int data_020a2f64;

void *func_0200ead8(void *object)
{
    func_0200a38c(object);
    *(int *)object = (int)&data_020a2f64;
    return object;
}
