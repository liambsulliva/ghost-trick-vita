void func_02052cec(void *object);
extern int data_020ac654;

void *func_02052d70(void *object)
{
    func_02052cec(object);
    *(int *)object = (int)&data_020ac654;
    return object;
}
