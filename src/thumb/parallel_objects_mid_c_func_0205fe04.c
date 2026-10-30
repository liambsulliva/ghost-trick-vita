extern long long func_0207b674();
extern int func_0205fe28();

void func_0205fe04(int param_1, int param_2)
{
    long long value;

    value = func_0207b674((long long)param_1, (long long)param_2);
    value = (value + 0x800) >> 12;
    func_0205fe28(value);
    return;
}
