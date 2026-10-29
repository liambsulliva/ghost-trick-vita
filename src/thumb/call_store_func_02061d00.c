void func_02061d78(void *object);
extern int data_020af9a8;

void *func_02061d00(void *object)
{
    func_02061d78(object);
    *(int *)object = (int)&data_020af9a8;
    return object;
}
