extern int func_02050c28();
extern int func_02050c34();

unsigned int func_02050c10(int param_1)

{
    func_02050c28(*(unsigned int *)(param_1 + 0x38),*(unsigned int *)(param_1 + 4),
                                *(unsigned int *)(param_1 + 8));
    func_02050c34(*(unsigned int *)(param_1 + 0x38));
    return 0;
}
