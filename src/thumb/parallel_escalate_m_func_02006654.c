struct State
{
    unsigned int before[4];
    int data;
};

extern struct State data_020b18a0;
extern void func_02001fa0();
extern void func_02007a84();

void func_02006654(void)
{
    unsigned int *puVar2;
    unsigned int uVar1;
    int iVar3;
    puVar2 = (unsigned int *)(data_020b18a0.data + 0x4c50);
    if (((*puVar2 & 2) != 0) && ((*puVar2 & 1) != 0)) {
        if (puVar2[6] != 0) {
            func_02001fa0(puVar2[6], puVar2[8]);
        }
        if (puVar2[7] != 0) {
            func_02001fa0(puVar2[7], puVar2[8]);
        }
        iVar3 = puVar2[13];
        if (iVar3 >= 0) {
            uVar1 = 1 << iVar3;
        } else {
            uVar1 = 0;
        }
        func_02007a84(puVar2[11], puVar2[12], uVar1, 0);
    }
}
