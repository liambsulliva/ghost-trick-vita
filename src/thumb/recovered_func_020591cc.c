extern int func_020266dc();
extern int func_020266f4();
extern int func_02026700();

void func_020591cc(unsigned int param_1,unsigned int param_2,unsigned int param_3)

{
    unsigned int uVar1;
    int iVar2;

    uVar1 = func_020266f4();
    if ((param_2 < uVar1) && (iVar2 = func_020266dc(param_1,param_2), iVar2 != 0)) {
        func_02026700(iVar2,param_3);
    }
    return;
}
