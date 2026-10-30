extern int func_02075c98();
extern void func_02075cac(int param_1);
extern int func_02074d08(int param_1, int param_2);
extern int func_02074d30(int param_1, int param_2);
extern int data_020d1188;

void func_02074ee0(int param_1, int param_2, unsigned int param_3)
{
    int *piVar1;
    int iVar2;
    int iVar3;
    int iVar4;

    iVar4 = func_02075c98();
    piVar1 = ((int **)&data_020d1188)[param_1];
    if (param_2 < 0) {
        param_2 = *piVar1;
    }
    param_3 = param_3 - 32;
    iVar3 = param_2 * 12 + piVar1[4];
    iVar2 = func_02074d08(*(int *)(iVar3 + 8), param_3);
    *(int *)(iVar3 + 8) = iVar2;
    piVar1 = (int *)func_02074d30(*(int *)(iVar3 + 4), param_3);
    *(int **)(iVar3 + 4) = piVar1;
    func_02075cac(iVar4);
}
