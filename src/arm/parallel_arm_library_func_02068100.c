void func_02068100(int *param_1,int param_2)
{
    int iVar1;
    unsigned short uVar2;

    uVar2 = (unsigned short)(((int)(unsigned int)*(unsigned short *)(param_2 + 2) >> 8) & 0x7f);
    iVar1 = param_2 + 0x10;
    *param_1 = param_2 - uVar2;
    param_1[1] = *(int *)(param_2 + 4) + iVar1;
}
