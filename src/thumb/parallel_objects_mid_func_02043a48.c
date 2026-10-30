extern int func_02043400();
extern void func_02043fc0();

void func_02043a48(int param_1)
{
    unsigned int uVar1;
    unsigned int uVar2;
    unsigned int uVar4;
    int iVar3;

    uVar1 = 0;
    for (uVar2 = 0; uVar2 < 3; uVar2 = uVar2 + 1) {
        uVar4 = 3 - uVar2;
        iVar3 = func_02043400(param_1, (short)uVar4);
        if (iVar3 != 0) {
            uVar1 = uVar4 & 0xff;
            break;
        }
    }
    if (*(int *)(param_1 + 0x44) != 0) {
        func_02043fc0(*(int *)(param_1 + 0x44), uVar1);
    }
    return;
}
