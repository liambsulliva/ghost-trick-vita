void func_02022d70(void *object);
extern int data_020a56b4;

void *func_02031df4(void *object)
{
    func_02022d70(object);
    *(int *)object = (int)&data_020a56b4;
    return object;
}
