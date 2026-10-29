extern int func_02008828();
extern int func_0201123c();
extern int func_02011264();
extern int func_02022f04();

int * func_020380b4(int *param_1)

{
    unsigned int uVar1;
    int iVar2;
    int iVar3;

    *param_1 = 0;
    *(unsigned short *)(param_1 + 1) = 0;
    uVar1 = func_02011264();
    uVar1 = func_0201123c(uVar1,0);
    iVar2 = func_02022f04(uVar1,0xffffffff);
    if ((iVar2 != 0) && (iVar3 = func_02008828(iVar2,((unsigned int)0x020cdd3cu)), iVar3 != 0)) {
        *param_1 = iVar2;
    }
    return param_1;
}
