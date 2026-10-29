void func_02056978(void *object);
extern int data_020ab538;

void *func_0204e2dc(void *object)
{
    func_02056978(object);
    *(int *)object = (int)&data_020ab538;
    return object;
}
