extern int func_02008824();
extern unsigned char * func_0200883c();
extern unsigned char * func_02009c1c();
extern int func_0200cb5c();
extern long long func_0201e888();
extern int func_0202f1bc();

void func_02064a00(unsigned int param_1,unsigned int param_2,int param_3)

{
    unsigned char *puVar1;
    int *piVar2;
    unsigned int uVar3;
    unsigned int *puVar4;
    long long lVar5;

    puVar1 = func_0200883c();
    piVar2 = (int *)func_02008824(puVar1);
    lVar5 = func_0201e888(piVar2,0,param_2 & 0xffff,1,(int *)0x0);
    puVar1 = func_0200883c();
    puVar1 = (unsigned char *)func_02008824(puVar1);
    uVar3 = func_0200cb5c(puVar1);
    if (param_3 == 0) {
        puVar4 = (unsigned int *)func_02009c1c();
        func_0202f1bc(puVar4,(unsigned int)lVar5,0,uVar3,7);
        return;
    }
    if (param_3 == 1) {
        puVar4 = (unsigned int *)func_02009c1c();
        func_0202f1bc(puVar4,(unsigned int)lVar5,0,uVar3,0x15);
    }
    return;
}
