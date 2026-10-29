extern int func_02008824();
extern unsigned char * func_0200883c();
extern unsigned long long func_0201e7f8();

void func_0201a97c(unsigned int param_1,int param_2)

{
    unsigned char *puVar1;
    int *piVar2;
    unsigned int uVar3;
    int iVar4;
    unsigned int uVar5;
    unsigned char abStack_30 [32];

    puVar1 = func_0200883c();
    piVar2 = (int *)func_02008824(puVar1);
    func_0201e7f8(piVar2,0,7,abStack_30);
    uVar3 = *(unsigned int *)(param_2 + 0x18) >> 1;
    iVar4 = param_2 + *(int *)(param_2 + 0x14);
    for (uVar5 = 0; uVar5 < uVar3; uVar5 = uVar5 + 1 & 0xff) {
        *(unsigned short *)(iVar4 + uVar5 * 2) = *(unsigned short *)(abStack_30 + uVar5 * 2);
    }
    return;
}
