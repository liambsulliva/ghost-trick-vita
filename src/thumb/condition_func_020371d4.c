extern int func_02008444();
extern int func_0201f944();
extern int func_0201ff10();
extern int func_0207b694();

int func_020371d4(unsigned int param_1)

{
    unsigned int uVar1;
    int iVar2;
    int iVar3;
    int iVar4;

    iVar3 = 0;
    iVar4 = 0;
    uVar1 = func_02008444();
    iVar2 = func_0201ff10(uVar1,0);
    if (iVar2 != 0) {
        iVar3 = func_0201f944(iVar2,param_1);
    }
    if ((0 < iVar3) && (iVar4 = func_0207b694(iVar3,10), 3 <= iVar4)) {
        iVar4 = 2;
    }
    return iVar4;
}
