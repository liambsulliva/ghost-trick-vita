extern int func_02008398();
extern int func_020349e4();

void func_020349b0(unsigned char *param_1,unsigned int *param_2,unsigned int param_3)

{
    int iVar1;
    int iVar2;
    int iVar3;

    iVar1 = func_020349e4(*param_2,param_3);
    iVar2 = func_020349e4(param_2[1],param_3);
    iVar3 = func_020349e4(param_2[2],param_3);
    func_02008398(param_1,iVar1,iVar2,iVar3);
    return;
}
