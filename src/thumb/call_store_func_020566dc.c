void func_020566f0(void *object);
extern int data_020ad808;

void *func_020566dc(void *object)
{
    func_020566f0(object);
    *(int *)object = (int)&data_020ad808;
    return object;
}
