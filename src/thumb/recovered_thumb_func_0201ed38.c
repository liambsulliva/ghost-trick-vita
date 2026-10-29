extern int func_0201eb24();
extern int func_0201ed80();

void func_0201ed38(int param_1)

{
    int iVar1;
    int iVar2;
    unsigned int uVar3;
    int iVar4;

    iVar1 = ((int)0x00000608u);
    func_0201ed80(param_1 + 4 + (*(int *)(param_1 + ((int)0x00000608u)) + 1) * 0xc,
                                param_1 + 4 + *(int *)(param_1 + ((int)0x00000608u)) * 0xc);
    iVar2 = *(int *)(param_1 + iVar1) + 1;
    iVar4 = iVar2 * 0xc;
    *(int *)(param_1 + iVar1) = iVar2;
    iVar2 = param_1 + 4 + iVar4;
    *(int *)(param_1 + iVar1 + -4) = iVar2;
    *(unsigned int *)(param_1 + 4 + iVar4) = *(unsigned int *)(iVar2 + 4);
    uVar3 = func_0201eb24(param_1);
    *(unsigned int *)(*(int *)(param_1 + iVar1 + -4) + 8) = uVar3;
    return;
}
