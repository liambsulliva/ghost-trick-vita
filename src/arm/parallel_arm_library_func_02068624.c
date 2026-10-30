extern int func_0206838c();
extern int func_02068440();

void func_02068624(int param_1, int param_2, int param_3)
{
    unsigned int uVar1;

    if (param_2 == 0)
    {
        param_2 = 1;
    }
    uVar1 = (unsigned int)(param_2 + 3) & 0xfffffffc;
    if (param_3 < 0)
    {
        goto neg;
    }
    func_0206838c(param_1, uVar1, param_3);
    return;
neg:
    func_02068440(param_1, uVar1, -param_3);
}
