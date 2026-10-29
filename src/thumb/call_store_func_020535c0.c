void func_020535d4(void *object);
extern int data_020acae0;

void *func_020535c0(void *object)
{
    func_020535d4(object);
    *(int *)object = (int)&data_020acae0;
    return object;
}
