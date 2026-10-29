extern int func_02008350();

void func_02059204(int param_1,int param_2,int param_3,unsigned int param_4)

{
    int iVar1;

    iVar1 = func_02008350();
    if (iVar1 != 0) {
        param_3 = -param_3;
    }
    param_2 = param_2 * 0xc;
    *(int *)(param_1 + param_2 + 0xe4) = param_3;
    *(unsigned int *)(param_1 + param_2 + 0xe8) = param_4;
    *(unsigned int *)(param_1 + param_2 + 0xec) = 0;
    return;
}
