void func_02065504(void *object);
extern int data_020b02e0;

void *func_02065064(void *object)
{
    func_02065504(object);
    *(int *)object = (int)&data_020b02e0;
    return object;
}
