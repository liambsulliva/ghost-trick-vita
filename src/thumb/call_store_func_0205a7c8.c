void func_0205a7dc(void *object);
extern int data_020aec10;

void *func_0205a7c8(void *object)
{
    func_0205a7dc(object);
    *(int *)object = (int)&data_020aec10;
    return object;
}
