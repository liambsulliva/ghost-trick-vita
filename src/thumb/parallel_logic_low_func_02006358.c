struct State
{
    unsigned int before[4];
    int data;
};

extern struct State data_020b18a0;
extern int func_02000d9c();

int func_02006358(unsigned int param_1, int param_2)
{
    int iVar1;
    int iVar2;
    int iVar3;
    unsigned char b;
    iVar2 = (int)&data_020b18a0;
    iVar1 = *(int *)(*(int *)(iVar2 + 0x10) + 0x1580);
    iVar1 = func_02000d9c(iVar1, param_1, *(int *)(*(int *)(iVar1 + 0x98) + 0x24));
    if (iVar1 == 0) {
        return -5;
    }
    iVar3 = *(int *)(iVar2 + 0x10) + 0x3d28;
    iVar2 = (unsigned int)*(unsigned char *)(iVar1 + 6) * 0x3a4;
    if ((*(unsigned int *)(iVar3 + iVar2 + 0xe8) & 1) == 0) {
        return -6;
    }
    b = (unsigned char)param_2;
    *(unsigned char *)(iVar3 + iVar2 + 0xf2) = b;
    *(unsigned char *)(iVar3 + iVar2 + 0x60) = b;
    *(unsigned char *)(iVar3 + iVar2 + 0xf1) = *(unsigned char *)(iVar3 + iVar2 + 0xf1) | 1;
    return 0;
}
