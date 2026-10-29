void func_02052ca4(void *object);
extern int data_020ac5cc;

void *func_02052cec(void *object)
{
    func_02052ca4(object);
    *(int *)object = (int)&data_020ac5cc;
    return object;
}
