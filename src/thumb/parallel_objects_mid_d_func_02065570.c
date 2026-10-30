extern unsigned char * func_02009c1c();
extern int func_0202f1bc();

void func_02065570(int param_1,unsigned int param_2,unsigned int param_3,unsigned int param_4)

{
    unsigned char bVar1;
    unsigned short uVar2;
    unsigned int *puVar3;
    unsigned int iVar4;
    unsigned short *puVar5;
    unsigned short *puVar6;
    unsigned int uVar7;
    unsigned short auStack_1c [4];
    unsigned int uStack_14;

    puVar5 = auStack_1c;
    iVar4 = 4;
    puVar6 = ((unsigned short *)0x0209e008u);
    uStack_14 = param_4;
    do {
        uVar2 = *puVar6;
        puVar6 = puVar6 + 1;
        *puVar5 = uVar2;
        puVar5 = puVar5 + 1;
        iVar4 = iVar4 + -1;
    } while (iVar4 != 0);
    uVar7 = *(unsigned int *)(param_1 + 4);
    if ((uVar7 != 0) && (bVar1 = *(unsigned char *)(param_1 + 0x14), bVar1 < 4)) {
        puVar3 = (unsigned int *)func_02009c1c();
        func_0202f1bc(puVar3,uVar7,0,*(unsigned int *)(param_1 + 0xc),(unsigned int)auStack_1c[bVar1]);
    }
    return;
}
