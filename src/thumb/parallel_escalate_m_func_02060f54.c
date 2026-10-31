extern void func_020083c0();
extern void func_0201f0a0();
extern void func_02058a80();
extern void func_020083b0();
extern void func_020349b0();
extern void func_02008378();

void func_02060f54(unsigned char *param_1, int param_2)
{
    unsigned int uVar1;
    int iVar2;
    unsigned char auStack_20[12];
    unsigned char auStack_2c[12];
    unsigned char auStack_38[12];
    unsigned char auStack_44[12];
    unsigned char auStack_50[12];

    func_020083c0(auStack_20, param_1 + 0x98, *(unsigned char **)(param_1 + 0xd0));
    func_0201f0a0(auStack_2c, auStack_20);
    if (*(int *)(param_1 + 0xfc) != 0) {
        func_02058a80(auStack_38, *(int *)(param_1 + 0xfc));
        func_020083b0(*(unsigned char **)(param_1 + 0xd0), auStack_38);
        if (param_2 != 0) {
            func_020083b0(*(unsigned char **)(param_1 + 0xd8), *(unsigned char **)(param_1 + 0xd0));
        }
    }
    for (uVar1 = 1; uVar1 < *(unsigned char *)(param_1 + 0x7f); uVar1 = (uVar1 + 1) & 0xff) {
        iVar2 = uVar1 * 0xc;
        func_020349b0(auStack_50, auStack_2c, uVar1 * 0x52);
        func_02008378(auStack_44, *(unsigned char **)(param_1 + 0xd0) + ((uVar1 - 1) & 0xff) * 0xc, auStack_50);
        func_020083b0(*(unsigned char **)(param_1 + 0xd0) + iVar2, auStack_44);
        if (param_2 != 0) {
            func_020083b0(*(unsigned char **)(param_1 + 0xd8) + iVar2, *(unsigned char **)(param_1 + 0xd0) + iVar2);
        }
    }
    return;
}
