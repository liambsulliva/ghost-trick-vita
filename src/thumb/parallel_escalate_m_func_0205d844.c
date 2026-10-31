int func_0205d820();
int func_0205d834();
int func_0206f28c();

void func_0205d844(int param_1, int *param_2, int *param_3, int param_4, unsigned short *param_5, int *param_6, int *param_7, int *param_8)
{
    int iVar1;
    int iVar2;
    unsigned int uVar3;
    unsigned int uVar4;
    unsigned int uVar5;

    iVar1 = func_0205d820((unsigned int)*param_5);
    iVar2 = func_0205d834((unsigned int)*param_5);
    uVar3 = func_0206f28c(*param_6, iVar1);
    uVar4 = func_0206f28c(*param_6, iVar2);
    uVar4 = func_0206f28c(uVar4, 0xc00);
    uVar5 = func_0206f28c(uVar3, *param_7) - func_0206f28c(uVar4, *param_8);
    uVar3 = func_0206f28c(uVar3, *param_8);
    uVar4 = func_0206f28c(uVar4, *param_7);
    *param_2 = *param_3 + uVar5;
    param_2[1] = param_3[1] + (uVar3 + uVar4);
}
