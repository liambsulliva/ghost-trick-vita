void func_020537e8(void *object);
extern int data_020ace54;

void *func_020537ac(void *object)
{
    func_020537e8(object);
    *(int *)object = (int)&data_020ace54;
    return object;
}
