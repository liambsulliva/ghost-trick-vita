extern int func_0205163c();

unsigned int func_02050c48(int param_1)

{
    func_0205163c(*(unsigned int *)(param_1 + 0x38),*(unsigned int *)(param_1 + 4),
                              *(unsigned int *)(param_1 + 8));
    *(int *)(param_1 + 8) = *(int *)(param_1 + 8) + 1;
    return 1;
}
