void func_0202ffb4(void *object);
extern int data_020ad900;

void *func_0205663c(void *object)
{
    func_0202ffb4(object);
    *(int *)object = (int)&data_020ad900;
    return object;
}
