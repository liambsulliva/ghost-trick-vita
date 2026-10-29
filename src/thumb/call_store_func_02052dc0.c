void func_02052dd4(void *object);
extern int data_020ac434;

void *func_02052dc0(void *object)
{
    func_02052dd4(object);
    *(int *)object = (int)&data_020ac434;
    return object;
}
