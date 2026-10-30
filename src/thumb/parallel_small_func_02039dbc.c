extern int func_02039fc8();
extern int func_0203a0dc();
extern int *func_02039e8c();
extern int func_02031254();
extern int func_02039ee4();
extern unsigned int * func_02009900();
extern int func_0202f7e8();

void func_02039dbc(int *param_1,int param_2,int param_3)

{
    int iVar1;
    int *piVar2;
    unsigned int *puVar3;
    int *puVar4;

    if (*param_1 == 0) {
        func_02039fc8(param_1,param_2,2);
        return;
    }
    iVar1 = func_0203a0dc(param_1,param_3);
    if (iVar1 == 0) {
        piVar2 = func_02039e8c(param_1);
        if (piVar2 != (int *)0x0) {
            iVar1 = func_02031254(0xc);
            *piVar2 = iVar1;
            if (iVar1 != 0) {
                *(int **)(iVar1 + 4) = piVar2;
                puVar4 = (int *)*piVar2;
                *puVar4 = 0;
                puVar4[2] = 0;
                puVar4[2] = func_02039ee4(param_1,param_2,param_3,0);
                puVar3 = func_02009900();
                func_0202f7e8((int)puVar3,puVar4[2],7,0);
            }
        }
    }
    return;
}
