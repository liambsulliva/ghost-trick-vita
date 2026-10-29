void func_020521b4(void *object);
extern int data_020abf98;

void *func_020521a0(void *object)
{
    func_020521b4(object);
    *(int *)object = (int)&data_020abf98;
    return object;
}
