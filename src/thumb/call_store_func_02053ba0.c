void func_02033e48(void *object);
extern int data_020ae70c;

void *func_02053ba0(void *object)
{
    func_02033e48(object);
    *(int *)object = (int)&data_020ae70c;
    return object;
}
