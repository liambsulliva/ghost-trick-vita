void func_0202694c(void *object);
extern int data_020a4b94;

void *func_020269e0(void *object)
{
    func_0202694c(object);
    *(int *)object = (int)&data_020a4b94;
    return object;
}
