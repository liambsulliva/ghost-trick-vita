void func_020520f0(void *object);
extern int data_020ac028;

void *func_020520dc(void *object)
{
    func_020520f0(object);
    *(int *)object = (int)&data_020ac028;
    return object;
}
