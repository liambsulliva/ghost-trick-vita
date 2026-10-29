void func_02035ed8(void *object);
extern int data_020a629c;

void *func_02036114(void *object)
{
    func_02035ed8(object);
    *(int *)object = (int)&data_020a629c;
    return object;
}
