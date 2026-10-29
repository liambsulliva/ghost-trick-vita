void func_02065504(void *object);
extern int data_020b02fc;

void *func_02065048(void *object)
{
    func_02065504(object);
    *(int *)object = (int)&data_020b02fc;
    return object;
}
