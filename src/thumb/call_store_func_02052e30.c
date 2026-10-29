void func_02052cec(void *object);
extern int data_020ac764;

void *func_02052e30(void *object)
{
    func_02052cec(object);
    *(int *)object = (int)&data_020ac764;
    return object;
}
