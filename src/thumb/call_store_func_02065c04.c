void func_020673c8(void *object);
extern int data_020b0640;

void *func_02065c04(void *object)
{
    func_020673c8(object);
    *(int *)object = (int)&data_020b0640;
    return object;
}
