void func_020299ec(unsigned int *param_1)

{
    int iVar1;

    iVar1 = ((int)0x0000040cu);
    *param_1 = 0;
    param_1[1] = 0;
    param_1[2] = 0;
    *(unsigned short *)((int)param_1 + iVar1) = 0;
    *(unsigned short *)((int)param_1 + iVar1 + 2) = 0;
    return;
}
