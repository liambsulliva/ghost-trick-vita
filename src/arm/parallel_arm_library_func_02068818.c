extern int func_020687c0();
extern int func_02068764();

void func_02068818(int param_1, int param_2, int param_3, int param_4)
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
    func_02068764((unsigned int *)(param_1 + 0x24), uVar1, param_3);
    return;
neg:
    func_020687c0((unsigned int *)(param_1 + 0x24), uVar1, -param_3);
}
