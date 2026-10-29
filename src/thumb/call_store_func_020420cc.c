void func_0200ca28(void *object);
extern int data_020a7de8;

void *func_020420cc(void *object)
{
    func_0200ca28(object);
    *(int *)object = (int)&data_020a7de8;
    return object;
}
