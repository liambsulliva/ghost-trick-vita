extern int func_02008824();
extern unsigned char * func_0200883c();
extern long long func_0201e888();

long long func_0200cdb0(unsigned int param_1,unsigned int param_2)

{
    unsigned char *puVar1;
    int *piVar2;
    long long lVar3;

    puVar1 = func_0200883c();
    piVar2 = (int *)func_02008824(puVar1);
    lVar3 = func_0201e888(piVar2,3,param_2,1,(int *)0x0);
    return lVar3;
}
