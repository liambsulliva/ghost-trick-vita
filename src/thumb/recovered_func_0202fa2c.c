void func_0202fa2c(int param_1,int param_2,int param_3)

{
    int iVar1;

    iVar1 = param_1 + 0x24 + param_3 * 0x10;
    *(unsigned int *)(param_2 + 4) = 0;
    if (*(int *)(iVar1 + 4) == 0) {
        *(unsigned int *)(param_2 + 8) = 0;
        *(int *)(iVar1 + 4) = param_2;
    }
    else {
        *(unsigned int *)(param_2 + 8) = *(unsigned int *)(iVar1 + 8);
        *(int *)(*(int *)(iVar1 + 8) + 4) = param_2;
    }
    *(int *)(iVar1 + 8) = param_2;
    return;
}
