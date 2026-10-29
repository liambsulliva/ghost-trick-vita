typedef int code();
extern int func_0201e230();

int func_0201e1cc(int *param_1,int param_2)

{
    int iVar1;

    iVar1 = (**(code **)(*param_1 + 8))(param_1,-param_2,((unsigned int)0x02084dd0u));
    if (iVar1 != 0) {
        func_0201e230(iVar1,1);
    }
    return iVar1;
}
