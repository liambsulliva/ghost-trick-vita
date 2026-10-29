void func_0200ca28(void *object);
extern int data_020a7db8;

void *func_0200ca14(void *object)
{
    func_0200ca28(object);
    *(int *)object = (int)&data_020a7db8;
    return object;
}
