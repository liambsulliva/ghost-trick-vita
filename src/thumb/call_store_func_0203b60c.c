void func_0203b5a8(void *object);
extern int data_020a7034;

void *func_0203b60c(void *object)
{
    func_0203b5a8(object);
    *(int *)object = (int)&data_020a7034;
    return object;
}
