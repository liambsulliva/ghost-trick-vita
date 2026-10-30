extern int func_02039fc8();
extern int func_0203a0dc();
extern int func_02031254();
extern int func_02039ee4();
extern unsigned int * func_02009900();
extern int func_0202f7e8();

void func_02039d58(int *param_1,int param_2,int param_3)

{
    int iVar1;
    int *piVar2;
    unsigned int *puVar3;

    if (*param_1 == 0) {
        func_02039fc8(param_1,param_2,2);
        return;
    }
    iVar1 = func_0203a0dc(param_1,param_3);
    if (iVar1 == 0) {
        piVar2 = (int *)*param_1;
        if (piVar2 != (int *)0x0 && piVar2[1] == 0) {
            iVar1 = func_02031254(0xc);
            piVar2[1] = iVar1;
            if (iVar1 != 0) {
                *(int **)iVar1 = piVar2;
                piVar2 = (int *)piVar2[1];
                *param_1 = (int)piVar2;
                piVar2[1] = 0;
                piVar2[2] = 0;
            }
            piVar2[2] = func_02039ee4(param_1,param_2,param_3,0);
            puVar3 = func_02009900();
            func_0202f7e8((int)puVar3,piVar2[2],7,0);
        }
    }
    return;
}
