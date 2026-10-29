void func_02056ac8(void *object);
extern int data_020a5304;

void *func_0202b1f4(void *object)
{
    func_02056ac8(object);
    *(int *)object = (int)&data_020a5304;
    return object;
}
