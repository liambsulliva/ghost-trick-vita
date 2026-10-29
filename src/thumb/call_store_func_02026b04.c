void func_0202694c(void *object);
extern int data_020a4c84;

void *func_02026b04(void *object)
{
    func_0202694c(object);
    *(int *)object = (int)&data_020a4c84;
    return object;
}
