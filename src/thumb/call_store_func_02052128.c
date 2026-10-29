void func_020520f0(void *object);
extern int data_020ac0b8;

void *func_02052128(void *object)
{
    func_020520f0(object);
    *(int *)object = (int)&data_020ac0b8;
    return object;
}
