void func_02026a1c(void *object);
extern int data_020a4b64;

void *func_02026a78(void *object)
{
    func_02026a1c(object);
    *(int *)object = (int)&data_020a4b64;
    return object;
}
