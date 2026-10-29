extern unsigned int * func_02008444();
extern int func_0201fd34();
extern int func_0201ff10();

unsigned int func_02037170(unsigned int param_1)

{
    unsigned int *puVar1;
    int iVar2;

    puVar1 = func_02008444();
    iVar2 = func_0201ff10((unsigned char *)puVar1,0);
    if ((iVar2 != 0) && (iVar2 = func_0201fd34(iVar2,param_1), iVar2 != 0)) {
        return 1;
    }
    return 0;
}
