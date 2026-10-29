void func_02035ed8(void *object);
extern int data_020a6500;

void *func_020361dc(void *object)
{
    func_02035ed8(object);
    *(int *)object = (int)&data_020a6500;
    return object;
}
