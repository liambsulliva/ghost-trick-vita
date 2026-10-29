void func_0202fe34(void *object);
extern int data_020a8a60;

void *func_020465ec(void *object)
{
    func_0202fe34(object);
    *(int *)object = (int)&data_020a8a60;
    return object;
}
