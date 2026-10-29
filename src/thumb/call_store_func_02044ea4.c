void func_02044ef4(void *object);
extern int data_020a87c4;

void *func_02044ea4(void *object)
{
    func_02044ef4(object);
    *(int *)object = (int)&data_020a87c4;
    return object;
}
