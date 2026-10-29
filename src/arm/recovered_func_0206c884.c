extern int func_02072fa8();
extern int func_02074844();
extern int func_020748b8();

void func_0206c884(int param_1)

{
    int iVar1;
    int iVar2;
    int iVar3;

    iVar1 = *(int *)(param_1 + 4);
    iVar2 = *(int *)(param_1 + 8);
    iVar3 = iVar2 + *(int *)(param_1 + 0xc);
    func_020748b8(iVar1,iVar3);
    func_02074844(iVar1,iVar3);
    func_02072fa8(iVar1 + iVar2,0,iVar3 - iVar2);
    return;
}
