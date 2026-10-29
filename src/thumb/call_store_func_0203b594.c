void func_0203b5a8(void *object);
extern int data_020a6fc4;

void *func_0203b594(void *object)
{
    func_0203b5a8(object);
    *(int *)object = (int)&data_020a6fc4;
    return object;
}
