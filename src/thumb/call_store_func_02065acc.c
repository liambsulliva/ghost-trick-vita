void func_02065c18(void *object);
extern int data_020a3024;

void *func_02065acc(void *object)
{
    func_02065c18(object);
    *(int *)object = (int)&data_020a3024;
    return object;
}
