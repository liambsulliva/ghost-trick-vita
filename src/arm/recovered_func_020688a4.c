void func_020688a4(int param_1,unsigned int *param_2)

{
    *param_2 = *(unsigned int *)(param_1 + 0x24);
    *(unsigned int **)(param_1 + 0x24) = param_2;
    return;
}
