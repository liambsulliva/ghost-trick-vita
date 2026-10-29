void func_02013398(void *object);
extern int data_020a6f34;

void *func_0203b634(void *object)
{
    func_02013398(object);
    *(int *)object = (int)&data_020a6f34;
    return object;
}
