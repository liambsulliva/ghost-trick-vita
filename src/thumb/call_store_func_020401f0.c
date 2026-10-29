void func_020404ac(void *object);
extern int data_020a7a0c;

void *func_020401f0(void *object)
{
    func_020404ac(object);
    *(int *)object = (int)&data_020a7a0c;
    return object;
}
