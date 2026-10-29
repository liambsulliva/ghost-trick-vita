void func_02007aa8(void *object);
extern int data_020a32c8;

void *func_02013398(void *object)
{
    func_02007aa8(object);
    *(int *)object = (int)&data_020a32c8;
    return object;
}
