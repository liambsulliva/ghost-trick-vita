void func_02053be8(void *object);
extern int data_020ad1d8;

void *func_02053bd4(void *object)
{
    func_02053be8(object);
    *(int *)object = (int)&data_020ad1d8;
    return object;
}
