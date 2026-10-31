extern char data_0209bf44;
extern unsigned short *func_0200eb78();
extern int func_02022ca4();
extern void func_02050320();
extern void func_0205032c();
extern void func_02050334();
extern void func_02050370();
extern void func_02050394();
extern int func_020503a0();
extern void func_020503c8();
extern void func_020503e4();
extern void func_0206e9d4();
extern void func_0206f77c();

void func_020501f4(int param_1, unsigned int param_2, int param_3, int param_4, int param_5, unsigned short param_6, int param_7, int param_8, unsigned int param_9)
{
    int iVar6;
    int sVar1;
    int sVar2;
    int sVar3;
    int sVar4;
    unsigned short *puVar5;
    int iVar11;
    int iVar7;
    int iVar12;
    int iVar13;
    int iVar14;
    unsigned int uVar8;
    unsigned int uVar9;
    unsigned int uVar10;
    int aiStack_44[12];

    if (param_7 >= 1) {
        if (0x1f < param_7) {
            param_7 = 0x1f;
        }
        puVar5 = func_0200eb78();
        iVar11 = func_02022ca4(puVar5, param_6);
        func_02050320(iVar11);
        iVar6 = param_5 >> 1;
        iVar7 = iVar6 * 3 >> 2;
        iVar11 = param_4 * 3 >> 2;
        func_0205032c(aiStack_44);
        iVar12 = (int)param_2 / 3;
        func_0206e9d4(aiStack_44, aiStack_44, param_1 >> 7, iVar12 >> 5, -param_3);
        func_0206f77c(aiStack_44);
        uVar10 = 0;
        func_02050334(0, 0, 0, 0, 0, 0, 0, 0);
        func_02050370(0xf, 0, 3, param_8, param_7, 0);
        func_02050394(2);
        iVar13 = iVar11 - iVar7;
        iVar14 = param_4 - iVar6;
        do {
            if ((int)param_9 <= 1 || uVar10 % param_9 == 0) {
                uVar8 = uVar10;
                if (uVar10 >= 0x20) {
                    uVar8 = uVar10 - 0x20;
                }
                uVar9 = *(unsigned int *)((int)&data_0209bf44 + uVar8 * 8 + 4);
                sVar1 = func_020503a0(iVar13, uVar9);
                uVar8 = *(unsigned int *)((int)&data_0209bf44 + uVar8 * 8);
                sVar2 = func_020503a0(iVar14, uVar8);
                sVar3 = func_020503a0(iVar11 + iVar7, uVar9);
                sVar4 = func_020503a0(param_4 + iVar6, uVar8);
                func_020503c8((short)sVar1, (short)sVar2, 0);
                func_020503c8((short)sVar3, (short)sVar4, 0);
            }
            uVar10 = uVar10 + 1;
        } while (uVar10 < 0x21);
        func_020503e4();
    }
}
