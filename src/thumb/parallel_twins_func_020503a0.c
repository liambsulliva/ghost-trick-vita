extern long long func_0207b674(long long param_1,long long param_2);
extern void func_020503c4(long long param_1);

void func_020503a0(int param_1,int param_2)
{
    long long value;

    value = func_0207b674((long long)param_1,(long long)param_2);
    value = (value + 0x800) >> 12;
    func_020503c4(value);
}
