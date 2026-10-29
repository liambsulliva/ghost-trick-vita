void func_0204ed6c(void *object);
extern int data_020ab908;

void *func_0204ed58(void *object)
{
    func_0204ed6c(object);
    *(int *)object = (int)&data_020ab908;
    return object;
}
