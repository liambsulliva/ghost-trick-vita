extern int func_0203a500();

void func_02038808(int param_1,int param_2)

{
    if ((*(int *)(param_1 + 0x1c) != 0) || (param_2 != -1)) {
        if (*(int *)(param_1 + 0x1c) + 1U < *(unsigned int *)(param_1 + 0x18)) {
            return;
        }
        if (param_2 != 1) {
            return;
        }
    }
    func_0203a500(*(unsigned int *)(param_1 + 0xc));
    return;
}
