void func_02056ac8(void *object);
extern int data_020ada10;

void *func_0205668c(void *object)
{
    func_02056ac8(object);
    *(int *)object = (int)&data_020ada10;
    return object;
}
