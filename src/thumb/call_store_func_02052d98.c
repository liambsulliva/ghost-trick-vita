void func_02052cec(void *object);
extern int data_020ac4bc;

void *func_02052d98(void *object)
{
    func_02052cec(object);
    *(int *)object = (int)&data_020ac4bc;
    return object;
}
