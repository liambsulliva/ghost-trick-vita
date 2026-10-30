int func_0201f138(int param_1,int param_2,int param_3,int param_4)
{
    int iVar1;

    iVar1 = param_2 - param_1;
    if (iVar1 >= 0) {
        return (((iVar1 << 1) / param_4) - param_3) / param_4;
    }
    iVar1 = (-iVar1 << 1) / param_4;
    iVar1 = (param_3 + iVar1) / param_4;
    return -iVar1;
}
