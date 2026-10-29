void func_02038aa4(void *object);
extern int data_020a6d1c;

void *func_02038a90(void *object)
{
    func_02038aa4(object);
    *(int *)object = (int)&data_020a6d1c;
    return object;
}
