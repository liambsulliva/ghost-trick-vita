void func_02052cec(void *object);
extern int data_020ac874;

void *func_02052d20(void *object)
{
    func_02052cec(object);
    *(int *)object = (int)&data_020ac874;
    return object;
}
