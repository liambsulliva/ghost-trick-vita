void func_0203e768(void *object);
extern int data_020a7528;

void *func_0203e754(void *object)
{
    func_0203e768(object);
    *(int *)object = (int)&data_020a7528;
    return object;
}
