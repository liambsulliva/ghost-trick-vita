void func_0202694c(void *object);
extern int data_020a4b04;

void *func_02026a1c(void *object)
{
    func_0202694c(object);
    *(int *)object = (int)&data_020a4b04;
    return object;
}
