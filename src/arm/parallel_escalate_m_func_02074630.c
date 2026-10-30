extern int func_02075c98();
extern void func_02075cac(int param_1);
extern void func_0207479c(int param_1, int param_2);
extern int data_020d0f28;

int func_02074630(int *param_1)
{
    int iVar1;
    int iVar2;
    int iVar3;
    unsigned int uVar4;

    iVar2 = func_02075c98();
    iVar1 = ((int *)&data_020d0f28)[1];
    if (param_1[2] == 0) {
        param_1[2] = iVar1;
        uVar4 = (unsigned int)param_1[3];
        uVar4 = (uVar4 & 0xffffff) | 0x10000000;
        param_1[3] = (int)((uVar4 & 0xff000000) | ((uVar4 + 1) & 0xffffff));
        func_0207479c(iVar1, (int)param_1);
        iVar3 = 1;
    }
    else if (param_1[2] == iVar1) {
        iVar3 = 1;
        uVar4 = (unsigned int)param_1[3];
        param_1[3] = (int)((uVar4 & 0xff000000) | ((uVar4 + 1) & 0xffffff));
    }
    else {
        iVar3 = 0;
    }
    func_02075cac(iVar2);
    return iVar3;
}
