extern int func_0203a004();

unsigned int func_02039ea0(int *param_1)

{
    func_0203a004(param_1);
    if (param_1[1] != 0) {
        return *(unsigned int *)(param_1[1] + 8);
    }
    return 0;
}
