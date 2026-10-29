void func_020537e8(void *object);
extern int data_020acd44;

void *func_020537d4(void *object)
{
    func_020537e8(object);
    *(int *)object = (int)&data_020acd44;
    return object;
}
