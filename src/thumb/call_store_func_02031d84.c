void func_02031d98(void *object);
extern int data_020a57bc;

void *func_02031d84(void *object)
{
    func_02031d98(object);
    *(int *)object = (int)&data_020a57bc;
    return object;
}
