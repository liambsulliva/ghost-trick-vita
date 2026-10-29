extern int func_0201f810();
extern int func_0201f934();

unsigned int func_0201fa5c(int param_1,int param_2,unsigned int param_3)

{
    int iVar1;

    iVar1 = func_0201f934();
    if (iVar1 == 0) {
        return 0;
    }
    iVar1 = *(int *)(*(int *)(param_1 + 0xc) + param_2 * 4);
    if ((iVar1 == 0) || (iVar1 = func_0201f810(iVar1,param_3), iVar1 != 0)) {
        return 1;
    }
    return 0;
}
