extern int func_02011254();
extern int func_02014c08();
extern int func_020549c4();

void func_02014bdc(int param_1,int param_2)

{
    int iVar1;

    *(int *)(param_1 + 0xc) = param_2;
    iVar1 = func_02011254();
    if ((((iVar1 != 0) && (param_2 == 0)) && (*(int *)(*(int *)(param_1 + 4) + 0x44) != 0)) &&
          (iVar1 = func_02014c08(), iVar1 != 0)) {
        func_020549c4();
    }
    return;
}
