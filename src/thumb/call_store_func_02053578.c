void func_0205358c(void *object);
extern int data_020acb6c;

void *func_02053578(void *object)
{
    func_0205358c(object);
    *(int *)object = (int)&data_020acb6c;
    return object;
}
