void func_02013398(void *object);
extern int data_020a5bc8;

void *func_02034428(void *object)
{
    func_02013398(object);
    *(int *)object = (int)&data_020a5bc8;
    return object;
}
