void func_02007aa8(void *object);
extern int data_020a4844;

void *func_0201f5e4(void *object)
{
    func_02007aa8(object);
    *(int *)object = (int)&data_020a4844;
    return object;
}
