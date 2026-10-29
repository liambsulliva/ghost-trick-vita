extern int func_02008444();
extern int func_0201fbc8();
extern int func_0201fd34();
extern int func_0201ff10();
extern int func_02037130();

unsigned int func_02037194(void)

{
    unsigned int uVar1;
    int iVar2;
    int iVar3;
    unsigned int uVar4;
    unsigned int uVar5;

    uVar1 = func_02008444();
    uVar5 = 0;
    iVar2 = func_0201ff10(uVar1,0);
    if (iVar2 != 0) {
        for (; uVar4 = func_0201fbc8(iVar2), uVar5 < uVar4; uVar5 = uVar5 + 1) {
            uVar1 = func_02037130(uVar5 & 0xff);
            iVar3 = func_0201fd34(iVar2,uVar1);
            if (iVar3 != 0) {
                return uVar5;
            }
        }
    }
    return 0xffffffff;
}
