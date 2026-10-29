unsigned int func_02022f04(int param_1,int param_2)

{
    if (param_2 < 0) {
        param_2 = *(int *)(param_1 + 0x24) + -1;
    }
    return *(unsigned int *)(param_1 + param_2 * 4 + 4);
}
