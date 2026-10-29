void func_02038aa4(void *object);
extern int data_020a6b64;

void *func_0203a66c(void *object)
{
    func_02038aa4(object);
    *(int *)object = (int)&data_020a6b64;
    return object;
}
