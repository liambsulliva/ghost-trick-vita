extern int func_0201003c();
extern int func_02015b58();
extern int func_02043400();
extern int func_02043424();
extern int func_02043474();
extern int func_020436c8();

unsigned int func_0204372c(int param_1)
{
    unsigned short uVar1;
    int iVar2;
    unsigned int uVar3;
    int iVar4;
    int iVar5;
    unsigned int uVar6;

    iVar5 = 0x100;
    iVar2 = func_02043400(param_1, -1);
    if (iVar2 == 0) {
        iVar5 = -1;
    }
    iVar2 = func_02043400(param_1, 3);
    if (iVar2 == 0) {
        iVar5 = 3;
    }
    if (iVar5 == 0x100) {
        return 0;
    }
    uVar6 = 0xffff;
    if (iVar5 == -1) {
        iVar2 = func_02043474(param_1);
        if ((iVar2 == 0) || (*(unsigned short *)(iVar2 + 0xe) == 0)) goto code_r0x020437a4;
        uVar1 = *(unsigned short *)(iVar2 + 0xe) - 1;
    }
    else {
        if ((iVar5 != 3) || (iVar2 = func_02043424(param_1), iVar2 == 0)) goto code_r0x020437a4;
        uVar3 = func_0201003c();
        iVar4 = func_02015b58(uVar3, *(unsigned short *)(iVar2 + 0xe) + 1);
        if (iVar4 == 0) goto code_r0x020437a4;
        uVar1 = *(unsigned short *)(iVar2 + 0xe) + 1;
    }
    uVar6 = (unsigned int)uVar1;
code_r0x020437a4:
    if (uVar6 != 0xffff) {
        func_020436c8(param_1, iVar5, uVar6);
    } else {
        return 0;
    }
    return 1;
}
