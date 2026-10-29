void func_02052cec(void *object);
extern int data_020ac7ec;

void *func_02052d48(void *object)
{
    func_02052cec(object);
    *(int *)object = (int)&data_020ac7ec;
    return object;
}
