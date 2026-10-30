extern int func_02075c98();
extern int func_020740d0();
extern int func_02075cac();

int func_0206b350(int param_1, int param_2)
{
    unsigned int uVar1;
    int iVar2;

    if (param_2 == 0x100)
    {
        iVar2 = func_02075c98();
        uVar1 = *(unsigned int *)(param_1 + 0xc);
        if ((uVar1 & 8) == 0)
        {
            do
            {
                func_020740d0(param_1 + 0x18);
                uVar1 = *(unsigned int *)(param_1 + 0xc);
            } while ((uVar1 & 8) == 0);
        }
        func_02075cac(iVar2);
        uVar1 = *(unsigned int *)(param_1 + 0xc);
        param_2 = *(int *)(param_1 + 0x14);
        *(unsigned int *)(param_1 + 0xc) = uVar1 & 0xfffffff7;
    }
    return param_2;
}
