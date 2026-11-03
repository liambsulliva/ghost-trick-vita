typedef int code();
extern int func_0200883c();
extern int func_02008824();
extern long long func_0201e888();
extern int func_02007bd8();
extern int func_02007b70();
extern int func_0207303c();
extern int func_02009c1c();
extern int func_0202f1bc();

int func_0203f254(int param_1, unsigned int param_2)
{
    unsigned int uVar18;
    unsigned int rows;
    unsigned short uVar1;
    unsigned short uVar2;
    int *piVar4;
    unsigned short *puVar5;
    int iVar6;
    unsigned short *puVar7;
    unsigned int *puVar8;
    unsigned int uVar9;
    unsigned int uVar10;
    int iVar11;
    int uVar12;
    unsigned short sVar13;
    unsigned int uVar14;
    unsigned int uVar15;
    unsigned int uVar16;
    int iVar17;

    piVar4 = (int *)func_02008824(func_0200883c());
    puVar5 = (unsigned short *)(unsigned int)func_0201e888(piVar4, 4, param_2, 1, 0);
    if (puVar5 == (unsigned short *)0x0) {
        uVar12 = 0;
    }
    else {
        uVar1 = *puVar5;
        *(unsigned int *)(param_1 + 0x40) = (unsigned int)uVar1;
        uVar9 = (unsigned int)(puVar5[1] << 17) >> 17;
        *(unsigned int *)(param_1 + 0x44) = uVar9;
        if ((uVar1 != 0x100) || (uVar9 != 0xc0)) {
            *(unsigned int *)(param_1 + 0x40) = 0;
            *(unsigned int *)(param_1 + 0x44) = 0;
            return 0;
        }
        else {
            iVar6 = func_02007bd8();
            piVar4 = (int *)func_02007b70(iVar6);
            puVar7 = (unsigned short *)(**(code **)(*piVar4 + 0xc))(piVar4, 0x200);
            if (puVar7 != (unsigned short *)0x0) {
                func_0207303c(puVar5 + 0x100, puVar7, 0x200);
                puVar8 = (unsigned int *)func_02009c1c();
                func_0202f1bc(puVar8, (unsigned int)puVar7, 0x6000, 0x200, 0x15);
            }
            uVar16 = *(int *)(param_1 + 0x40) * *(int *)(param_1 + 0x44);
            iVar6 = func_02007bd8();
            piVar4 = (int *)func_02007b70(iVar6);
            puVar7 = (unsigned short *)(**(code **)(*piVar4 + 0xc))(piVar4, uVar16);
            if (puVar7 != (unsigned short *)0x0) {
                func_0207303c((unsigned short *)((char *)puVar5 + 0x200) + 0x100, puVar7, uVar16);
                puVar8 = (unsigned int *)func_02009c1c();
                func_0202f1bc(puVar8, (unsigned int)puVar7, 0, uVar16, 0x19);
            }
            if (*(char *)(param_1 + 0x50) == 0) {
                iVar6 = func_02007bd8();
                piVar4 = (int *)func_02007b70(iVar6);
                uVar16 = (**(code **)(*piVar4 + 0xc))(piVar4, 0x800);
                if (uVar16 != 0) {
                    uVar18 = *(unsigned int *)(param_1 + 0x40) >> 3;
                    sVar13 = 0;
                    uVar10 = *(unsigned int *)(param_1 + 0x44);
                    rows = uVar10 >> 3;
                    for (uVar14 = 0; uVar14 < rows; uVar14 = uVar14 + 2) {
                        iVar11 = uVar16 + uVar14 * 0x40;
                        iVar6 = uVar16 + (uVar14 + 1) * 0x40;
                        for (uVar15 = 0; uVar15 < uVar18; uVar15 = uVar15 + 2) {
                            iVar17 = uVar15 * 2;
                            *(unsigned short *)(iVar11 + iVar17) = sVar13;
                            *(unsigned short *)(iVar11 + iVar17 + 2) = sVar13 + 1;
                            *(unsigned short *)(iVar6 + iVar17) = sVar13 + 2;
                            *(unsigned short *)(iVar6 + iVar17 + 2) = sVar13 + 3;
                            sVar13 = sVar13 + 4;
                        }
                    }
                    puVar8 = (unsigned int *)func_02009c1c();
                    func_0202f1bc(puVar8, uVar16, 0, 0x800, 0x1d);
                    *(unsigned char *)(param_1 + 0x50) = 1;
                }
            }
            return 1;
        }
    }
    return uVar12;
}
