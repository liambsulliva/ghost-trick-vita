void func_02026a1c(void *object);
extern int data_020a4a8c;

void *func_02026adc(void *object)
{
    func_02026a1c(object);
    *(int *)object = (int)&data_020a4a8c;
    return object;
}
