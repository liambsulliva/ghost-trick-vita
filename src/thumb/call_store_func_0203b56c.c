void func_0203b4f4(void *object);
extern int data_020a6ffc;

void *func_0203b56c(void *object)
{
    func_0203b4f4(object);
    *(int *)object = (int)&data_020a6ffc;
    return object;
}
