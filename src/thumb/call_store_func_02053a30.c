void func_02052cec(void *object);
extern int data_020acee8;

void *func_02053a30(void *object)
{
    func_02052cec(object);
    *(int *)object = (int)&data_020acee8;
    return object;
}
