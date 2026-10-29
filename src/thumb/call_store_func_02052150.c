void func_020520f0(void *object);
extern int data_020ac070;

void *func_02052150(void *object)
{
    func_020520f0(object);
    *(int *)object = (int)&data_020ac070;
    return object;
}
