struct State
{
    unsigned int before[4];
    int data;
};
extern struct State data_020b18a0;
extern unsigned int func_0207b8a0();
extern unsigned int func_0200658c();
extern void func_02074188();
extern void func_02074844();
void func_020064f4(int param_1)
{
    unsigned int uVar1;
    int iVar2;
    int iVar3;
    int iVar4;
    iVar2 = (int)&data_020b18a0;
    if (*(int *)(*(int *)(iVar2 + 0x10) + 0x3d24) != 0) {
        if ((*(int *)(param_1 + 0x44) - *(int *)(param_1 + 0x48) < *(int *)(param_1 + 0x38) - 1) &&
            (uVar1 = func_0200658c(), (uVar1 & 1) == 0)) {
            func_02074188(*(int *)(iVar2 + 0x10) + 0x1fa4);
        }
        *(int *)(param_1 + 0x48) = *(int *)(param_1 + 0x48) + 1;
        return;
    }
    iVar2 = func_0207b8a0(*(unsigned int *)(param_1 + 0x20), *(unsigned int *)(param_1 + 0x38));
    iVar3 = iVar2 * *(int *)(param_1 + 0x24);
    uVar1 = 0;
    iVar4 = *(int *)(param_1 + 0x18);
    if (*(int *)(param_1 + 0x1c) != 0) {
        uVar1 = *(int *)(param_1 + 0x1c) + iVar3;
    }
    func_02074844(iVar4 + iVar3, iVar2);
    func_02074844(uVar1, iVar2);
    (*(void (**)())(param_1 + 0x3c))(iVar4 + iVar3, uVar1, iVar2, *(int *)(param_1 + 0xc), *(int *)(param_1 + 0x40));
    if (*(int *)(param_1 + 0x24) == *(int *)(param_1 + 0x38) - 1) {
        iVar2 = 0;
    } else {
        iVar2 = *(int *)(param_1 + 0x24) + 1;
    }
    *(int *)(param_1 + 0x24) = iVar2;
}
