extern char data_020b18a0;
extern int func_02000d9c();

int func_02006c90(unsigned int param_1)

{
    int iVar1;

    iVar1 = *(int *)(*(int *)(((int)&data_020b18a0) + 0x10) + 0x1580);
    iVar1 = func_02000d9c(iVar1,param_1,*(int *)(*(int *)(iVar1 + 0x98) + 8));
    if (iVar1 == 0) {
        return 0;
    }
    return iVar1 + 4;
}
