void func_02055868(void *object);
extern int data_020ad4f4;

void *func_020557c0(void *object)
{
    func_02055868(object);
    *(int *)object = (int)&data_020ad4f4;
    return object;
}
