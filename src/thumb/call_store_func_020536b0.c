void func_02052ca4(void *object);
extern int data_020acc04;

void *func_020536b0(void *object)
{
    func_02052ca4(object);
    *(int *)object = (int)&data_020acc04;
    return object;
}
