void func_02026a1c(void *object);
extern int data_020a4bc4;

void *func_02026a50(void *object)
{
    func_02026a1c(object);
    *(int *)object = (int)&data_020a4bc4;
    return object;
}
