void func_02033e48(void *object);
extern int data_020ae684;

void *func_020589cc(void *object)
{
    func_02033e48(object);
    *(int *)object = (int)&data_020ae684;
    return object;
}
