extern int func_02008824();
extern unsigned char * func_0200883c();
extern unsigned char * func_02008860();
extern unsigned char * func_0200b208();
extern int func_0200c820();
extern int func_0200cb5c();
extern int func_0200cb60();
extern long long func_0201e888();
extern unsigned char * func_02023d60();
extern int func_02023ebc();
extern int func_02023ef4();
extern int func_02023f34();
extern unsigned char * func_020420cc();
extern unsigned char * func_02042170();

unsigned char * func_020420e0(unsigned int param_1)

{
    unsigned char *puVar1;
    unsigned char *puVar2;
    int *piVar3;
    int *piVar4;
    unsigned char *puVar5;
    unsigned int uVar6;
    unsigned int uVar7;
    long long lVar8;

    uVar7 = (param_1 & 0xffff) * 0x400 | 0x4c000000;
    puVar1 = func_0200b208();
    puVar1 = (unsigned char *)func_02023ebc((int)puVar1,uVar7);
    if (puVar1 != (unsigned char *)0x0) {
        func_0200cb60(puVar1);
        return (unsigned char *)func_02042170(puVar1);
    }
    puVar1 = func_0200b208();
    func_02023ef4((int)puVar1);
    puVar1 = func_02023d60(0x14);
    if (puVar1 != (unsigned char *)0x0) {
        puVar1 = func_020420cc(puVar1);
    }
    if (puVar1 != (unsigned char *)0x0) {
        puVar2 = func_0200883c();
        piVar3 = (int *)func_02008824(puVar2);
        func_02008860();
        piVar4 = (int *)func_0200c820();
        lVar8 = func_0201e888(piVar3,3,param_1,0,piVar4);
        if ((int)lVar8 != 0) {
            puVar2 = func_0200b208();
            puVar5 = func_0200883c();
            puVar5 = (unsigned char *)func_02008824(puVar5);
            uVar6 = func_0200cb5c(puVar5);
            return (unsigned char *)func_02023f34((int)puVar2,(int)puVar1,(int)lVar8,uVar6,uVar7);
        }
    }
    return (unsigned char *)0x0;
}
