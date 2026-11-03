extern int func_02008444(void);
extern int func_020334ec(int p);
extern int func_020334f8(int p);
extern int func_0201230c(int p);
extern void func_020082d8(int *out, int p);
extern int func_02010f84(int p, int v);
extern void func_02008398(int *p, int x, int y, int z);

void func_02033428(int *param_1, int param_2, int *param_3, int param_4)
{
    int aPos[3];
    int iTemp;
    int iSx;
    int iSy;
    int iVar5;
    int iVar7;
    int iVar6;
    int iVar9;
    int iVar2;
    int iVar8;
    int dx;
    int dy;

    iVar9 = param_3[0] >> 12;
    iVar2 = param_3[1] >> 12;
    iVar8 = func_020334ec(func_02008444()) - 0x100;
    iTemp = func_020334f8(func_02008444()) - 0xc0;
    func_020082d8(aPos, func_0201230c(func_02008444()));
    iSx = aPos[0] >> 12;
    iSy = aPos[1] >> 12;
    iVar5 = iVar9 + iSx;
    iVar6 = iVar5 - 0x80;
    if (iVar6 < 0) {
        iVar6 = 0;
    }
    if (iVar6 > iVar8) {
        iVar6 = iVar8;
    }
    iVar7 = iVar2 + iSy;
    iVar2 = iVar7 - 0x60;
    if (iVar2 < 0) {
        iVar2 = 0;
    }
    if (iVar2 > iTemp) {
        iVar2 = iTemp;
    }
    if (func_02010f84(func_02008444(), 6) != 0 && param_4 != 0) {
        dx = iVar5 - iVar6;
        dy = iVar7 - iVar2;
        if (0x58 < dx && dx < 0xa8 && 0xa0 < dy && dy < 0xc0) {
            iVar5 = iVar9 - 0x58;
            iVar7 = iVar9 - 0xa8;
            if (iVar5 < iVar8 >> 1) {
                iVar6 = iVar5;
            } else {
                iVar6 = iVar7;
            }
        }
    }
    func_02008398(param_1, (iVar6 - iSx + 0x80) << 12, (iVar2 - iSy + 0x60) << 12, 0);
}
