void func_02035ed8(void *object);
extern int data_020a6434;

void *func_0203609c(void *object)
{
    func_02035ed8(object);
    *(int *)object = (int)&data_020a6434;
    return object;
}
