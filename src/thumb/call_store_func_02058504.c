void func_02058194(void *object);
extern int data_020ae420;

void *func_02058504(void *object)
{
    func_02058194(object);
    *(int *)object = (int)&data_020ae420;
    return object;
}
