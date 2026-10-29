void func_02043964(void *object);
extern int data_020a808c;

void *func_020439fc(void *object)
{
    func_02043964(object);
    *(int *)object = (int)&data_020a808c;
    return object;
}
