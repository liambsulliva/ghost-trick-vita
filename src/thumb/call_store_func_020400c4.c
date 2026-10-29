void func_0202fe34(void *object);
extern int data_020aed4c;

void *func_020400c4(void *object)
{
    func_0202fe34(object);
    *(int *)object = (int)&data_020aed4c;
    return object;
}
