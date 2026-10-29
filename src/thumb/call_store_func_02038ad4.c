void func_02038ae8(void *object);
extern int data_020a681c;

void *func_02038ad4(void *object)
{
    func_02038ae8(object);
    *(int *)object = (int)&data_020a681c;
    return object;
}
