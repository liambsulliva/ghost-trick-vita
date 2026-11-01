int func_0201d380(int param_1, unsigned int param_2, unsigned short *param_3)
{
    int iVar2;
    unsigned int uVar1;
    iVar2 = 0;
    while ((uVar1 = param_3[iVar2]) != 0xffff) {
        if (param_2 == uVar1) {
            return 1;
        }
        iVar2 = iVar2 + 1;
    }
    return 0;
}
