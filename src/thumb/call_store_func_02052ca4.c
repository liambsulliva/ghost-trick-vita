void func_02033e48(void *object);
extern int data_020ae684;

void *func_02052ca4(void *object)
{
    func_02033e48(object);
    *(int *)object = (int)&data_020ae684;
    return object;
}
