void func_02052cec(void *object);
extern int data_020ac544;

void *func_02052cd8(void *object)
{
    func_02052cec(object);
    *(int *)object = (int)&data_020ac544;
    return object;
}
