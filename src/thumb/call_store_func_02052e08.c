void func_02052cec(void *object);
extern int data_020ac6dc;

void *func_02052e08(void *object)
{
    func_02052cec(object);
    *(int *)object = (int)&data_020ac6dc;
    return object;
}
