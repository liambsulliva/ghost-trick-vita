void func_02053be8(void *object);
extern int data_020acfb8;

void *func_02053c1c(void *object)
{
    func_02053be8(object);
    *(int *)object = (int)&data_020acfb8;
    return object;
}
