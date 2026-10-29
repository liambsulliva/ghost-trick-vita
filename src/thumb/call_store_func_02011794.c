void func_02011774(void *object);
extern int data_020b10f8;

void *func_02011794(void *object)
{
    func_02011774(object);
    *(int *)object = (int)&data_020b10f8;
    return object;
}
