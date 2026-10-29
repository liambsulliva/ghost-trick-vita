void func_02065858(void *object);
extern int data_020b029c;

void *func_02064c54(void *object)
{
    func_02065858(object);
    *(int *)object = (int)&data_020b029c;
    return object;
}
