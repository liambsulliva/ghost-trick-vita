extern int func_02008860();
extern int func_0200affc();
extern int func_0200b00c();
extern int func_0200cb68();

void func_0200eff0(int param_1)

{
    unsigned int uVar1;
    int iVar2;

    uVar1 = func_02008860();
    iVar2 = func_0200b00c(uVar1,0);
    if (iVar2 != 0) {
        func_0200cb68(iVar2,*(unsigned int *)(param_1 + ((int)0x00000efcu)));
    }
    uVar1 = func_02008860();
    iVar2 = func_0200affc(uVar1,0);
    if (iVar2 != 0) {
        func_0200cb68(iVar2,*(unsigned int *)(param_1 + 0xf00));
    }
    iVar2 = ((int)0x00000efcu);
    *(unsigned int *)(param_1 + ((int)0x00000efcu)) = 0xffffffff;
    *(unsigned int *)(param_1 + iVar2 + 4) = 0xffffffff;
    return;
}
