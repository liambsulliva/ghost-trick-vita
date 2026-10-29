void func_02056ac8(void *object);
extern int data_020a3838;

void *func_020163a0(void *object)
{
    func_02056ac8(object);
    *(int *)object = (int)&data_020a3838;
    return object;
}
