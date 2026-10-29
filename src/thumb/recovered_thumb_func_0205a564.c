extern int func_0206f114();
extern int func_0206f174();
extern int func_0206f28c();
extern int func_0207b8a0();

void func_0205a564(unsigned int param_1,unsigned int *param_2,int *param_3,unsigned int *param_4,
                                    unsigned int *param_5,int param_6)

{
    unsigned int uVar1;
    int iVar2;

    uVar1 = func_0206f114(param_4);
    iVar2 = func_0207b8a0(uVar1,param_6);
    param_3[2] = iVar2;
    func_0206f174(param_4,param_4);
    iVar2 = func_0206f28c(*param_4,param_3[2]);
    *param_3 = -iVar2;
    iVar2 = func_0206f28c(param_4[1],param_3[2]);
    param_3[1] = -iVar2;
    param_3[2] = param_6 + -1;
    *param_2 = *param_5;
    param_2[1] = param_5[1];
    return;
}
