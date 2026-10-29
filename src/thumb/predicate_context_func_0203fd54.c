extern unsigned int * func_02008444();
extern int func_0201fcf4();
extern int func_0201ff10();

unsigned int func_0203fd54(unsigned int param_1,unsigned int param_2)

{
    unsigned int *puVar1;
    int iVar2;
    int iVar3;
    int iVar4;

    iVar3 = 0;
    puVar1 = func_02008444();
    iVar2 = func_0201ff10((unsigned char *)puVar1,2);
    if (iVar2 != 0) {
        iVar3 = func_0201fcf4(iVar2,param_2);
    }
    iVar4 = 0;
    puVar1 = func_02008444();
    iVar2 = func_0201ff10((unsigned char *)puVar1,0);
    if (iVar2 != 0) {
        iVar4 = func_0201fcf4(iVar2,param_2);
    }
    if (!((iVar3 == 0) && (iVar4 == 0))) {
        return 1;
    }
    return 0;
}
