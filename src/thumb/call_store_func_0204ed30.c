void func_0204eddc(void *object);
extern int data_020ab9f4;

void *func_0204ed30(void *object)
{
    func_0204eddc(object);
    *(int *)object = (int)&data_020ab9f4;
    return object;
}
