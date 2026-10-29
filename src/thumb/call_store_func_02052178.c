void func_020520f0(void *object);
extern int data_020abfe0;

void *func_02052178(void *object)
{
    func_020520f0(object);
    *(int *)object = (int)&data_020abfe0;
    return object;
}
