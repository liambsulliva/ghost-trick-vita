extern int func_0200d88c();

unsigned int func_0200d84c(int param_1,int param_2)

{
    int iVar1;

    if (**(char **)(param_1 + 8) == '\0') {
        iVar1 = func_0200d88c(*(char **)(param_2 + 4),*(char **)(param_1 + 4));
        if (iVar1 == 0) {
            return 1;
        }
        return 0;
    }
    iVar1 = func_0200d88c(*(char **)(param_2 + 8),*(char **)(param_1 + 8));
    if ((iVar1 == 0) &&
          (iVar1 = func_0200d88c(*(char **)(param_2 + 4),*(char **)(param_1 + 4)), iVar1 == 0)) {
        return 1;
    }
    return 0;
}
