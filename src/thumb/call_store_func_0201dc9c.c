void func_020145dc(void *object);
extern int data_020e47f0;

void *func_0201dc9c(void *object)
{
    func_020145dc(object);
    *(int *)object = (int)&data_020e47f0;
    return object;
}
