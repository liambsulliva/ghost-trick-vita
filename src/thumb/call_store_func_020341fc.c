void func_02022d70(void *object);
extern int data_020a5b00;

void *func_020341fc(void *object)
{
    func_02022d70(object);
    *(int *)object = (int)&data_020a5b00;
    return object;
}
