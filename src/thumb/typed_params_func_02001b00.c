extern char data_020b18a0;
extern int func_02000d9c();
extern int func_02001b54();

unsigned int func_02001b00(unsigned int param_1,unsigned int param_2)

{
    int iVar1;
    int *piVar2;
    int iVar3;
    unsigned int uVar4;

    iVar3 = ((int)&data_020b18a0);
    iVar1 = *(int *)(*(int *)(((int)&data_020b18a0) + 0x10) + 0x1580);
    piVar2 = (int *)func_02000d9c(iVar1,param_1,*(int *)(*(int *)(iVar1 + 0x98) + 0x10));
    if (piVar2 == (int *)0x0) {
        return 0xfffffffb;
    }
    uVar4 = *(int *)(*(int *)(*(int *)(*(int *)(iVar3 + 0x10) + 0x1580) + 0x90) + *piVar2 * 0x10 +
                                    0x10) + 0x1fU & 0xffffffe0;
    if ((param_2 & 4) != 0) {
        iVar3 = func_02001b54(param_1);
        uVar4 = uVar4 + iVar3;
    }
    return uVar4;
}
