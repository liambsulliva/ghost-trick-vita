extern int func_02038790();
extern int func_02038bd4();
extern int func_02038c04();
extern int func_02039e1c();
extern int func_02039e54();
extern int func_02039eb8();

void func_020386f4(unsigned int *param_1, int param_2)
{
    int iVar1;
    int iVar2;
    int zero;
    int m5;

    if (param_1[0xb] == 0) {
        iVar2 = 2;
        param_1[10] = iVar2;
        func_02039e1c(*param_1, iVar2 - 3);
        func_02039e54(*param_1, 5);
        func_02038c04(param_1);
        func_02038bd4(param_1, param_1[6], param_2);
    }
    param_1[0xb] = param_1[0xb] + param_2;
    *(signed char *)(param_1 + 0xc) = (signed char)param_2;
    iVar1 = func_02038790(*param_1);
    if (iVar1 != 0) {
        zero = 0;
        func_02039eb8(*param_1, 4, zero);
        m5 = 5;
        func_02039eb8(*param_1, 5, param_2);
        iVar1 = param_1[0xb];
        if ((iVar1 == 4) || (iVar1 == m5 - 9)) {
            param_1[0xb] = 0;
            *(unsigned char *)((int)param_1 + 0x31) = 1;
            *(signed char *)(param_1 + 0xc) = (signed char)param_2;
            return;
        }
        if (iVar1 == 0) {
            *(unsigned char *)((int)param_1 + 0x31) = zero;
            *(unsigned char *)(param_1 + 0xc) = zero;
            func_02039eb8(*param_1, 1, (int)*(signed char *)(param_1 + 0xc));
        }
    }
    return;
}
