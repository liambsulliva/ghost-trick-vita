extern int func_02050900();

void func_02050ea4(int param_1,int param_2)

{
    int iVar1;

    for (iVar1 = *(int *)(param_1 + 0x3c); (param_2 != 0 && (iVar1 != 0));
            iVar1 = *(int *)(iVar1 + 0x54)) {
        func_02050900(iVar1,param_2);
    }
    return;
}
