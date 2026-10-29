void func_0202f524(void *object);
extern int data_020abd4c;

void *func_02051c10(void *object)
{
    func_0202f524(object);
    *(int *)object = (int)&data_020abd4c;
    return object;
}
