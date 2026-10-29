void func_02007aa8(void *object);
extern int data_020a2fcc;

void *func_0200e5c0(void *object)
{
    func_02007aa8(object);
    *(int *)object = (int)&data_020a2fcc;
    return object;
}
