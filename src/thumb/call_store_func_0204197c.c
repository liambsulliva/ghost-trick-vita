void func_020145dc(void *object);
extern int data_020d4928;

void *func_0204197c(void *object)
{
    func_020145dc(object);
    *(int *)object = (int)&data_020d4928;
    return object;
}
