void func_02013398(void *object);
extern int data_020a32a8;

void *func_020133cc(void *object)
{
    func_02013398(object);
    *(int *)object = (int)&data_020a32a8;
    return object;
}
