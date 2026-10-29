void func_02056978(void *object);
extern int data_020a5c1c;

void *func_02034400(void *object)
{
    func_02056978(object);
    *(int *)object = (int)&data_020a5c1c;
    return object;
}
