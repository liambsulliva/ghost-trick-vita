void func_02063bc0(void *object);
extern int data_020affc0;

void *func_02063bac(void *object)
{
    func_02063bc0(object);
    *(int *)object = (int)&data_020affc0;
    return object;
}
