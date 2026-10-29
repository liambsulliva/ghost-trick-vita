void func_02031d20(void *object);
extern int data_020a6834;

void *func_02037044(void *object)
{
    func_02031d20(object);
    *(int *)object = (int)&data_020a6834;
    return object;
}
