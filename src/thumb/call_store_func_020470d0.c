void func_02056ac8(void *object);
extern int data_020a8cd4;

void *func_020470d0(void *object)
{
    func_02056ac8(object);
    *(int *)object = (int)&data_020a8cd4;
    return object;
}
