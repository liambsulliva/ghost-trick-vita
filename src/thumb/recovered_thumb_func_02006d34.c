extern char data_020b18a0;
extern int func_02000d9c();

int func_02006d34(unsigned int param_1,int param_2)

{
    int iVar1;
    int *piVar2;
    int iVar3;

    iVar3 = ((int)&data_020b18a0);
    iVar1 = *(int *)(*(int *)(((int)&data_020b18a0) + 0x10) + 0x1580);
    piVar2 = (int *)func_02000d9c(iVar1,param_1,*(unsigned int *)(*(int *)(iVar1 + 0x98) + 0xc));
    if (piVar2 == (int *)0x0) {
        return 0;
    }
    iVar3 = *(int *)(*(int *)(*(int *)(*(int *)(iVar3 + 0x10) + 0x1580) + 0x90) + *piVar2 * 0x10 +
                                    0x14);
    if (iVar3 == 0) {
        return 0;
    }
    iVar3 = iVar3 + 0x20 + param_2 * 0xc;
    if (iVar3 == 0) {
        return 0;
    }
    return iVar3 + 4;
}
