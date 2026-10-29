extern char data_02083c58;
unsigned int func_0200e104(int param_1,int param_2)

{
    *(unsigned int *)(param_1 + 4) = 4;
    if (param_2 < 3) {
        *(int *)(param_1 + 4) = param_2;
        *(unsigned int *)(param_1 + 0x14) = *(unsigned int *)(((unsigned int)&data_02083c58) + param_2 * 8);
    }
    if (*(int *)(param_1 + 4) == 4) {
        return 0;
    }
    return 1;
}
