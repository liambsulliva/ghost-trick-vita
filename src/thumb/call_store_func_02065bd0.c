void func_020673c8(void *object);
extern int data_020b0950;

void *func_02065bd0(void *object)
{
    func_020673c8(object);
    *(int *)object = (int)&data_020b0950;
    return object;
}
