void func_02052ca4(void *object);
extern int data_020acc98;

void *func_0205371c(void *object)
{
    func_02052ca4(object);
    *(int *)object = (int)&data_020acc98;
    return object;
}
