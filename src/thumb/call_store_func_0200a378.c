void func_0200a38c(void *object);
extern int data_020a2e84;

void *func_0200a378(void *object)
{
    func_0200a38c(object);
    *(int *)object = (int)&data_020a2e84;
    return object;
}
