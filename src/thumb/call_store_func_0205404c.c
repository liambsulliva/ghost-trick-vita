void func_0202fe34(void *object);
extern int data_020ad37c;

void *func_0205404c(void *object)
{
    func_0202fe34(object);
    *(int *)object = (int)&data_020ad37c;
    return object;
}
