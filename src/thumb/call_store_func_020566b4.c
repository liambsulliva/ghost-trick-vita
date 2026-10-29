void func_0202f524(void *object);
extern int data_020ad840;

void *func_020566b4(void *object)
{
    func_0202f524(object);
    *(int *)object = (int)&data_020ad840;
    return object;
}
