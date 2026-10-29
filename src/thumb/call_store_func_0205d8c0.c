void func_020569f0(void *object);
extern int data_020af208;

void *func_0205d8c0(void *object)
{
    func_020569f0(object);
    *(int *)object = (int)&data_020af208;
    return object;
}
