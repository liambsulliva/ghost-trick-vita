extern int func_02008444();
extern int func_0200f5c8();
extern unsigned int func_0200f5e8();
extern void func_0200f70c();
extern void func_020083c0();
extern void func_020083b0();
extern void func_0206083c();
extern void func_0202bcd4();
extern int func_020452e8();
extern char data_020a46b0;

void func_020608d8(unsigned char *param_1, int param_2, int param_3, int param_4)
{
    int auStack_20[3];
    int auStack_2c[3];
    int auStack_38[3];
    int auStack_44[3];
    int auStack_50[3];
    int puVar6;
    unsigned int uVar5;
    int iVar3;
    int iVar4;
    int puVar1;
    int uVar2;

    puVar6 = *(int *)(param_1 + 0xe4);
    for (uVar5 = 0; uVar5 < *(unsigned char *)(param_1 + 0x7f); uVar5 = (uVar5 + 1) & 0xff) {
        iVar4 = uVar5 * 0xc;
        puVar1 = func_02008444();
        iVar3 = func_0200f5c8(puVar1);
        *(int *)(*(int *)(param_1 + 0xe4) + iVar4) =
            (*(int *)(*(int *)(param_1 + 0xd0) + iVar4) - iVar3) - 0x80000;
        puVar1 = func_02008444();
        *(int *)(*(int *)(param_1 + 0xe4) + iVar4 + 4) =
            -*(int *)(*(int *)(param_1 + 0xd0) + iVar4 + 4) + func_0200f5e8(puVar1) + 0x60000;
        *(int *)(*(int *)(param_1 + 0xe4) + iVar4 + 8) = 0;
    }
    for (uVar5 = 0; uVar5 < *(unsigned char *)(param_1 + 0x7f); uVar5 = (uVar5 + 1) & 0xff) {
        func_0200f70c(auStack_20);
        func_0200f70c(auStack_2c);
        if ((int)uVar5 < (int)(*(unsigned char *)(param_1 + 0x7f) - 1)) {
            if (uVar5 == 0) {
                func_020083c0(auStack_44, puVar6 + 0x18, puVar6);
                func_020083b0(auStack_20, auStack_44);
            }
            else {
                func_020083c0(auStack_50, puVar6 + (uVar5 + 1) * 0xc, puVar6 + uVar5 * 0xc);
                func_020083b0(auStack_20, auStack_50);
            }
            func_020083b0(auStack_2c, &data_020a46b0);
            auStack_2c[2] = 0;
            auStack_20[2] = 0;
            func_0206083c(*(int *)(param_1 + 0xe8) + uVar5 * 0x24, (int)auStack_2c, (int)auStack_20);
        }
        else {
            param_4 = uVar5 - 1;
            func_0202bcd4(*(int *)(param_1 + 0xe8) + uVar5 * 0x24, *(int *)(param_1 + 0xe8) + param_4 * 0x24);
        }
    }
    iVar3 = *(unsigned char *)(param_1 + 0x7f) - 1;
    puVar6 = *(int *)(param_1 + 0xd0);
    func_020083c0(auStack_38, puVar6 + iVar3 * 0xc, puVar6);
    uVar2 = func_020452e8(auStack_38);
    *(int *)(param_1 + 0xa8) = uVar2;
}
