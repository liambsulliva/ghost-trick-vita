void func_02035ed8(void *object);
extern int data_020a6324;

void *func_020360ec(void *object)
{
    func_02035ed8(object);
    *(int *)object = (int)&data_020a6324;
    return object;
}
