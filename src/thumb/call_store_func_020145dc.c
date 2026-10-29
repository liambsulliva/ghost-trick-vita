void func_02007aa8(void *object);
extern int data_020a3400;

void *func_020145dc(void *object)
{
    func_02007aa8(object);
    *(int *)object = (int)&data_020a3400;
    return object;
}
