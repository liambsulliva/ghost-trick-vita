void func_02035ed8(void *object);
extern int data_020a6544;

void *func_020361b4(void *object)
{
    func_02035ed8(object);
    *(int *)object = (int)&data_020a6544;
    return object;
}
