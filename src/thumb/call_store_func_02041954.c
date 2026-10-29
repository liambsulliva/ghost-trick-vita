void func_020145dc(void *object);
extern int data_020d4908;

void *func_02041954(void *object)
{
    func_020145dc(object);
    *(int *)object = (int)&data_020d4908;
    return object;
}
