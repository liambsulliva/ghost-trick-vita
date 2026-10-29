void func_02016420(void *object);
extern int data_020a355c;

void *func_02016484(void *object)
{
    func_02016420(object);
    *(int *)object = (int)&data_020a355c;
    return object;
}
