void func_02022d70(void *object);
extern int data_020a7560;

void *func_0203e768(void *object)
{
    func_02022d70(object);
    *(int *)object = (int)&data_020a7560;
    return object;
}
