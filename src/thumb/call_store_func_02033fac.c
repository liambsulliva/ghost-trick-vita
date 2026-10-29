void func_02056978(void *object);
extern int data_020a5968;

void *func_02033fac(void *object)
{
    func_02056978(object);
    *(int *)object = (int)&data_020a5968;
    return object;
}
