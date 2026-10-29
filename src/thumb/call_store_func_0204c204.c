void func_0204c27c(void *object);
extern int data_020ab26c;

void *func_0204c204(void *object)
{
    func_0204c27c(object);
    *(int *)object = (int)&data_020ab26c;
    return object;
}
