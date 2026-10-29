void func_02033e48(void *object);
extern int data_020ae9d4;

void *func_0205a5e4(void *object)
{
    func_02033e48(object);
    *(int *)object = (int)&data_020ae9d4;
    return object;
}
