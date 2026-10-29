void func_02047120(void *object);
extern int data_020aa7e4;

void *func_0204c268(void *object)
{
    func_02047120(object);
    *(int *)object = (int)&data_020aa7e4;
    return object;
}
