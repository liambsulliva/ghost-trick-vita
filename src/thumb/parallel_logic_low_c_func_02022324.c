unsigned int func_02022324(int param_1)

{
    unsigned int uVar1;

    for (uVar1 = 0; uVar1 < (unsigned int)(*(int *)(param_1 + 8) - 1); uVar1 = uVar1 + 1) {
        if (*(signed char *)(*(int *)(param_1 + 4) + uVar1) == 0) {
            if (*(signed char *)(*(int *)(param_1 + 4) + uVar1 + 1) == 0) {
                return uVar1;
            }
        }
    }
    return 0;
}
