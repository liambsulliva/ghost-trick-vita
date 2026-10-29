void func_02022d70(void *object);
extern int data_020a564c;

void *func_02031c64(void *object)
{
    func_02022d70(object);
    *(int *)object = (int)&data_020a564c;
    return object;
}
