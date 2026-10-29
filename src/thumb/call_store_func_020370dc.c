void func_020370f0(void *object);
extern int data_020a68bc;

void *func_020370dc(void *object)
{
    func_020370f0(object);
    *(int *)object = (int)&data_020a68bc;
    return object;
}
