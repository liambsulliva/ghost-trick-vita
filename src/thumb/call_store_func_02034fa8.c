void func_02011774(void *object);
extern int data_020a5d94;

void *func_02034fa8(void *object)
{
    func_02011774(object);
    *(int *)object = (int)&data_020a5d94;
    return object;
}
