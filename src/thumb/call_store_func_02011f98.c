void func_02011e1c(void *object);
extern int data_020a4fd0;

void *func_02011f98(void *object)
{
    func_02011e1c(object);
    *(int *)object = (int)&data_020a4fd0;
    return object;
}
