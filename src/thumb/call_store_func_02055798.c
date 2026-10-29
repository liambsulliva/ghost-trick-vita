void func_02055868(void *object);
extern int data_020ad474;

void *func_02055798(void *object)
{
    func_02055868(object);
    *(int *)object = (int)&data_020ad474;
    return object;
}
