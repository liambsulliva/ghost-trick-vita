struct State
{
    unsigned int before[4];
    int data;
};

extern struct State data_020b18a0;
extern void func_02074188();
extern unsigned int func_0207bd24();
extern unsigned int func_0207bab8();
extern void func_0200685c();
extern void func_02006938();

void func_02006598(void)
{
    unsigned int uVar1;
    int iVar2;
    unsigned int *puVar3;
    unsigned int *puVar5;
    iVar2 = data_020b18a0.data;
    puVar3 = (unsigned int *)(iVar2 + 0x4c50);
    if (*(int *)(iVar2 + 0x3d24) != 0) {
        func_02074188(iVar2 + 0x1fa4);
    }
    if ((*puVar3 & 8) != 0) {
        puVar5 = puVar3 + 0x13;
        uVar1 = func_0207bd24(puVar5[1], puVar5[2]);
        puVar5[1] = uVar1;
        uVar1 = func_0207bd24(0x3f000000, uVar1);
        uVar1 = func_0207bab8(uVar1);
        puVar3[0x19] = uVar1;
        if (puVar5[0] == 0) {
            puVar3[0x19] = *((unsigned char *)puVar5 + 0xc);
            if (puVar5[5] == 1) {
                func_0200685c();
            }
            *puVar3 = *puVar3 & 0xfffffff7;
            puVar5[5] = 3;
        } else {
            puVar5[0] = puVar5[0] - 1;
        }
        func_02006938(puVar3[0x19]);
    }
}
