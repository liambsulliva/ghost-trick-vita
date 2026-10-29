void func_02013398(void *object);
extern int data_020a562c;

void *func_02031e1c(void *object)
{
    func_02013398(object);
    *(int *)object = (int)&data_020a562c;
    return object;
}
