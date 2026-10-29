extern int func_020230d4();
extern int func_020230d8();
extern int func_020230dc();

void func_02023094(int param_1)

{
    int iVar1;
    int current;

    if (*(int *)(param_1 + 0x28) != 0) {
        current = *(int *)(param_1 + *(int *)(param_1 + 0x24) * 4);
        if ((*(int *)(param_1 + 0x24) == 0) ||
              ((current != 0 &&
                (iVar1 = func_020230d4(), iVar1 != 0)))) {
            func_020230dc(param_1,*(unsigned int *)(param_1 + 0x28));
            *(unsigned int *)(param_1 + 0x28) = 0;
            iVar1 = *(int *)(param_1 + *(int *)(param_1 + 0x24) * 4);
            if (iVar1 != 0) {
                func_020230d8(iVar1,*(unsigned int *)(param_1 + 0x2c));
            }
        }
    }
    return;
}
