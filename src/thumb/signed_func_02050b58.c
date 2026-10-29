extern int func_0200da04();
extern int func_0200daac();
extern int func_02027c28();
extern int func_02050bc8();
extern int func_020790c8();

unsigned int func_02050b58(int param_1)

{
    signed char *pcVar1;
    unsigned int uVar2;
    unsigned int uVar3;
    unsigned int uVar4;
    int iVar5;

    iVar5 = *(int *)(param_1 + 4) + *(int *)(param_1 + 8) * 2;
    pcVar1 = (signed char *)func_02027c28(*(unsigned short *)(iVar5 + 2));
    uVar2 = func_02027c28(*(unsigned short *)(iVar5 + 4));
    uVar3 = func_02027c28(*(unsigned short *)(iVar5 + 6));
    if (*pcVar1 != '*') {
        uVar4 = func_0200daac(*(unsigned int *)(param_1 + 0x38));
        iVar5 = func_020790c8(uVar4,pcVar1);
        if ((iVar5 == 0) &&
              (iVar5 = func_02050bc8(*(unsigned int *)(param_1 + 0x38),uVar2,uVar3), iVar5 != 0)) {
            *(int *)(param_1 + 8) = *(int *)(param_1 + 8) + 4;
            return 1;
        }
    }
    uVar4 = func_0200daac(*(unsigned int *)(param_1 + 0x38));
    func_0200da04(pcVar1,uVar2,uVar3,uVar4);
    *(int *)(param_1 + 8) = *(int *)(param_1 + 8) + 4;
    return 1;
}
