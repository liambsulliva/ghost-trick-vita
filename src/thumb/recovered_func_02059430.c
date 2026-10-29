void func_02059430(int param_1,unsigned int param_2,unsigned char param_3,unsigned char param_4)

{
    *(unsigned int *)(param_1 + 0xd8) = param_2;
    *(unsigned char *)(param_1 + 0xdc) = param_3;
    *(unsigned char *)(param_1 + 0xdd) = param_4;
    *(unsigned char *)(param_1 + 0xde) = *(unsigned char *)(param_1 + 0xde) | 2;
    return;
}
