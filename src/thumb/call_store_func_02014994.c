void func_020149a8(void *object);
extern int data_020a34d8;

void *func_02014994(void *object)
{
    func_020149a8(object);
    *(int *)object = (int)&data_020a34d8;
    return object;
}
