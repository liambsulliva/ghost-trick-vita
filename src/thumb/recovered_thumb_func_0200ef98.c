extern int func_02008860();
extern int func_0200affc();
extern int func_0200b00c();
extern int func_0200cb04();

void func_0200ef98(int param_1)

{
    unsigned int uVar1;
    int iVar2;
    int iVar3;

    iVar3 = ((int)0x00000efcu);
    if (*(int *)(param_1 + ((int)0x00000efcu)) == -1) {
        uVar1 = func_02008860();
        iVar2 = func_0200b00c(uVar1,0);
        if (iVar2 != 0) {
            uVar1 = func_0200cb04(iVar2,0x83,0x80000000);
            *(unsigned int *)(param_1 + iVar3) = uVar1;
        }
    }
    if (*(int *)(param_1 + 0xf00) == -1) {
        uVar1 = func_02008860();
        iVar3 = func_0200affc(uVar1,0);
        if (iVar3 != 0) {
            uVar1 = func_0200cb04(iVar3,0x89,0x80000000);
            *(unsigned int *)(param_1 + 0xf00) = uVar1;
        }
    }
    return;
}
