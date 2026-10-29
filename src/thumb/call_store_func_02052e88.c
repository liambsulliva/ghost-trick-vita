void func_02052e9c(void *object);
extern int data_020ac8fc;

void *func_02052e88(void *object)
{
    func_02052e9c(object);
    *(int *)object = (int)&data_020ac8fc;
    return object;
}
