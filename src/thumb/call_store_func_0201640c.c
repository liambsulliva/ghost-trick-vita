void func_02016420(void *object);
extern int data_020a359c;

void *func_0201640c(void *object)
{
    func_02016420(object);
    *(int *)object = (int)&data_020a359c;
    return object;
}
