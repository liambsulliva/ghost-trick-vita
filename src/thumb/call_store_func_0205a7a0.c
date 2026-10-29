void func_02056ac8(void *object);
extern int data_020aeb34;

void *func_0205a7a0(void *object)
{
    func_02056ac8(object);
    *(int *)object = (int)&data_020aeb34;
    return object;
}
