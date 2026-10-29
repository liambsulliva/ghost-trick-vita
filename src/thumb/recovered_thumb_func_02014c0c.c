extern int func_02011254();
extern int func_02014c08();
extern int func_02014c40();
extern int func_02014c50();

unsigned int func_02014c0c(int param_1)

{
    int iVar1;
    unsigned int uVar2;

    iVar1 = func_02011254();
    if (iVar1 != 0) {
        if (*(int *)(*(int *)(param_1 + 4) + 0x48) != 0) {
            uVar2 = func_02014c40();
            return uVar2;
        }
        if ((*(int *)(*(int *)(param_1 + 4) + 0x44) != 0) && (iVar1 = func_02014c08(), iVar1 != 0)) {
            uVar2 = func_02014c50();
            return uVar2;
        }
    }
    return 0;
}
