struct State
{
    unsigned int before[4];
    int data;
};
struct Key
{
    unsigned int idx : 24;
    unsigned int hi : 8;
};

extern struct State data_020b18a0;
extern struct Key *func_02000d9c();
extern unsigned int func_02000d64();

int func_02002794(unsigned int param_1, unsigned int param_2)
{
    int iVar1;
    struct Key *puVar2;
    unsigned int uVar3;
    int iVar4;
    int iVar5;
    unsigned int uVar6;
    uVar6 = param_2;
    param_2 = param_2 + 0x1f;
    param_2 = param_2 & 0xffffffe0;
    iVar5 = (int)&data_020b18a0;
    iVar1 = *(int *)(*(int *)(iVar5 + 0x10) + 0x1580);
    puVar2 = func_02000d9c(iVar1, param_1, *(int *)(*(int *)(iVar1 + 0x98) + 0x14));
    iVar4 = *(int *)(*(int *)(iVar5 + 0x10) + 0x1580);
    iVar1 = *(int *)(iVar4 + 0x90);
    uVar3 = puVar2->idx;
    if (uVar3 >= *(unsigned int *)(iVar1 + 8)) {
        return -10;
    }
    iVar1 = iVar1 + uVar3 * 0x10;
    if (*(int *)(iVar1 + 0x14) == 0) {
        uVar3 = func_02000d64(iVar4, uVar3, param_2, *(unsigned int *)(iVar1 + 0x10), 0);
        iVar1 = *(int *)puVar2;
        iVar1 = *(int *)(*(int *)(*(int *)(iVar5 + 0x10) + 0x1580) + 0x90) + (iVar1 & 0xffffff) * 0x10;
        if (uVar3 < *(unsigned int *)(iVar1 + 0x10)) {
            return -2;
        }
        *(unsigned int *)(iVar1 + 0x14) = param_2;
        param_2 = param_2 + uVar3;
    }
    return param_2 - uVar6;
}
