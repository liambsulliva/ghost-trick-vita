void func_02043964(void *object);
extern int data_020a7fa8;

void *func_02043950(void *object)
{
    func_02043964(object);
    *(int *)object = (int)&data_020a7fa8;
    return object;
}
