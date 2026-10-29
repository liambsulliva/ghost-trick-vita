void func_02007aa8(void *object);
extern int data_020a49ec;

void *func_02023348(void *object)
{
    func_02007aa8(object);
    *(int *)object = (int)&data_020a49ec;
    return object;
}
