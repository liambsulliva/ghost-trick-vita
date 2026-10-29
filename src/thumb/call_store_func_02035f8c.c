void func_02035ed8(void *object);
extern int data_020a6698;

void *func_02035f8c(void *object)
{
    func_02035ed8(object);
    *(int *)object = (int)&data_020a6698;
    return object;
}
