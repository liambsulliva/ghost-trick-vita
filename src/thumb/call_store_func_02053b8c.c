void func_02053ba0(void *object);
extern int data_020ad260;

void *func_02053b8c(void *object)
{
    func_02053ba0(object);
    *(int *)object = (int)&data_020ad260;
    return object;
}
