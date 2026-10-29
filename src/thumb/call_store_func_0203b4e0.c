void func_0203b4f4(void *object);
extern int data_020a6f8c;

void *func_0203b4e0(void *object)
{
    func_0203b4f4(object);
    *(int *)object = (int)&data_020a6f8c;
    return object;
}
