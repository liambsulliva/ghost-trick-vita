void func_0204c27c(void *object);
extern int data_020aaf3c;

void *func_0204c16c(void *object)
{
    func_0204c27c(object);
    *(int *)object = (int)&data_020aaf3c;
    return object;
}
