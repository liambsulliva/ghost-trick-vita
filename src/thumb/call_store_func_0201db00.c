void func_02007aa8(void *object);
extern int data_020a4484;

void *func_0201db00(void *object)
{
    func_02007aa8(object);
    *(int *)object = (int)&data_020a4484;
    return object;
}
