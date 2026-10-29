extern int func_020511c0();

unsigned int func_02050fbc(int param_1)

{
    func_020511c0(*(unsigned int *)(param_1 + 0x34),*(unsigned int *)(param_1 + 4),
                              *(unsigned int *)(param_1 + 8));
    *(int *)(param_1 + 8) = *(int *)(param_1 + 8) + 1;
    return 1;
}
