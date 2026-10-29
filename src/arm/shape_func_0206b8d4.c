extern int func_0206b3a8();
extern int func_0206b728();
extern int func_02074120();
extern int func_02075c98();
extern int func_02075cac();

void func_0206b8d4(int param_1)

{
    unsigned int uVar1;
    int iVar2;
    unsigned int uVar3;
    unsigned int uVar4;

    uVar4 = *(unsigned int *)(param_1 + 8);
    if (param_1 == 0) {
        return;
    }
    do {
        uVar1 = func_02075c98();
        uVar3 = *(unsigned int *)(param_1 + 0xc) | 0x40;
        *(unsigned int *)(param_1 + 0xc) = uVar3;
        if ((uVar3 & 4) != 0) {
            func_02074120(param_1 + 0x18);
            param_1 = 0;
        }
        func_02075cac(uVar1);
        if (param_1 == 0) {
            return;
        }
        iVar2 = func_0206b3a8(param_1,*(unsigned int *)(param_1 + 0xc) >> 8 & 0xff);
        if (iVar2 == 0x100) {
            return;
        }
        param_1 = func_0206b728(uVar4,1);
    } while (param_1 != 0);
    return;
}
