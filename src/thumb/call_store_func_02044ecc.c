void func_02044ef4(void *object);
extern int data_020a8584;

void *func_02044ecc(void *object)
{
    func_02044ef4(object);
    *(int *)object = (int)&data_020a8584;
    return object;
}
