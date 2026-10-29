void func_02031d20(void *object);
extern int data_020a5784;

void *func_02031d0c(void *object)
{
    func_02031d20(object);
    *(int *)object = (int)&data_020a5784;
    return object;
}
