extern int func_0202f64c();
extern int func_02031270();
extern int func_02039e8c();
extern int func_0203a0c0();

void func_02039e54(unsigned int param_1,unsigned int param_2)

{
    int iVar1;
    int iVar2;

    iVar1 = func_02039e8c();
    if ((iVar1 != 0) &&
          (iVar2 = func_0203a0c0(param_1,*(unsigned int *)(iVar1 + 8),param_2), iVar2 != 0)) {
        **(unsigned int **)(iVar1 + 4) = 0;
        func_0202f64c(iVar1 + 8);
        if (iVar1 != 0) {
            func_02031270(iVar1);
        }
    }
    return;
}
