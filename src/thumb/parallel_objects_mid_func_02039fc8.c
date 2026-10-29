extern int func_02009900();
extern void func_0202f7e8();
extern int func_02039ee4();
extern int func_02039fb0();

void func_02039fc8(int *param_1,unsigned int param_2,unsigned int param_3)

{
    int iVar1;
    unsigned int uVar2;

    if (*param_1 == 0) {
        iVar1 = func_02039fb0(param_1);
        *param_1 = iVar1;
        if (iVar1 != 0) {
            uVar2 = func_02039ee4(param_1,param_2,param_3,0);
            *(unsigned int *)(*param_1 + 8) = uVar2;
            uVar2 = func_02009900();
            func_0202f7e8(uVar2,*(unsigned int *)(*param_1 + 8),7,0);
        }
    }
    return;
}
