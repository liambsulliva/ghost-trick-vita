void func_02035ed8(void *object);
extern int data_020a6368;

void *func_020360c4(void *object)
{
    func_02035ed8(object);
    *(int *)object = (int)&data_020a6368;
    return object;
}
